#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "wiringBbb.h"

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define SYSFS_PWM_DIR  "/sys/devices/ocp.3/pwm_test_"
#define POLL_TIMEOUT   5000 /* 5 seconds */
#define MAX_BUF        256

#define PWM_FIXED_FREQUENCY ((float)100.0e6)

#define PWM_CONTROL 0
#define PWM_STATUS  1
#define PWM0_RANGE  4
#define PWM0_DATA   5
#define PWM1_RANGE  8
#define PWM1_DATA   9

#ifndef	TRUE
#define	TRUE	(1==1)
#define	FALSE	(1==2)
#endif

// Number of pins
#define MAX_GPIO_ID 255

typedef enum {
  Shared,
  Greedy,
  Weak,
} gpioMode;

typedef enum {
  Digital,
  Pwm,
  Adc,
} gpioType;

typedef struct {
  int (* callback_fn)(void *);   /** Callback to be called on ISR. NOTE: Execution code shall be short as possible */
  void *callback_arg;
  int ready;                     /** Contains the pin value SysFS file descriptor, fd */
} gpioCallback;

typedef struct {
  int fd;                        /** pin value SysFS file descriptor */
  int fdd;                       /** direction SysFS file descriptor */
  int fdm;                       /** edge SysFS file descriptor */
  gpioCallback callback;         /** ISR callback */
} digitalDesc;

typedef struct {
  int fdp;                       /** PWM period SysFS file descriptor */
  int fddc;                      /** PWM Duty Cycle SysFS file descriptor */
  int fdr;                       /** PWM run/stop SysFS file descriptor */
  const char *pwmChip;           /** PWM chip number: 0: ephpwmA */
  unsigned int range;
  unsigned int clock;
  float period;                  /**  Period of the PWM signal (in ns) */
  float duty_cycle;              /** Duty cycle of the PWM signal (in ns) */
} pwmDesc;

typedef struct {
  unsigned int gpio;                     /** Gpio port number */
  int shared;                            /** Gpio shared counter. Default: 0 (not shared) */
  gpioType type;
  union {
    digitalDesc digital;
    pwmDesc pwm;                         /** The PWM chip set number */
  } types;
} gpioDesc;

static unsigned char isInit = FALSE;
static uint64_t epochMilli, epochMicro ; /** Time for easy calculations */
static gpioDesc * sysFds[MAX_GPIO_ID];   /** sysFds: Map a file descriptor from the /sys/class/xxx */
static pthread_t isrThreadId = -1;       /** GPIOs ISR thread identifier */
static struct pollfd isrFds[MAX_GPIO_ID / 2];   /** poll file descriptors table */
static int isrGpios[MAX_GPIO_ID / 2];    /** poll file descriptors table */
static int isrFdsNum;                    /** Number of file descriptors in the isrFds table */

static void initialiseEpoch(void);
static int allocGpio(const unsigned int p_gpio, const gpioMode p_gpioMode, const gpioType p_gpioType);
static int freeGpio(const unsigned int p_gpio);
static const char * pwmChipFromPin(const unsigned int p_gpio);
static void pwmUpdate (gpioDesc *p_gpio);

static void *interruptHandler (void * p_arg);

/*
 * wiringBbbFailure:
 * Fail.
 *********************************************************************************
 */
int wiringBbbFailure (int fatal, const char *message, ...)
{
  va_list argp ;
  char buffer [1024] ;

  va_start (argp, message) ;
  vsnprintf (buffer, 1023, message, argp) ;
  va_end (argp) ;

  fprintf (stderr, "%s", buffer) ;
  exit (EXIT_FAILURE) ;

  return 0 ;
}

/*
 * bbbBoardRev:
 * Return a number representing the hardware revision of the board.
 * Revision is currently 1 or 2.
 */
static void bbbBoardRevOops (const char *why)
{
  fprintf (stderr, "bbbBoardRev: Unable to determine board revision from /proc/cpuinfo\n") ;
  fprintf (stderr, " -> %s\n", why) ;
  fprintf (stderr, " ->  You may want to check:\n") ;
  fprintf (stderr, " ->  http://www.raspberrypi.org/phpBB3/viewtopic.php?p=184410#p184410\n") ;
  exit (EXIT_FAILURE) ;
}

int bbbBoardRev (void)
{
  FILE *cpuFd ;
  char line [120] ;
  char *c, lastChar ;
  static int  boardRev = -1 ;

  if (boardRev != -1)	// No point checking twice
    return boardRev ;

  if ((cpuFd = fopen ("/proc/cpuinfo", "r")) == NULL)
    bbbBoardRevOops ("Unable to open /proc/cpuinfo") ;

  while (fgets (line, 120, cpuFd) != NULL)
    if (strncmp (line, "Revision", 8) == 0)
      break ;

  fclose (cpuFd) ;

  if (strncmp (line, "Revision", 8) != 0)
    bbbBoardRevOops ("No \"Revision\" line") ;

  for (c = &line [strlen (line) - 1] ; (*c == '\n') || (*c == '\r') ; --c)
    *c = 0 ;
  
  for (c = line ; *c ; ++c)
    if (isdigit (*c))
      break ;

  if (!isdigit (*c))
    bbbBoardRevOops ("No numeric revision string") ;

  lastChar = line [strlen (line) - 1] ;

  /* if ((lastChar == '2') || (lastChar == '3'))
     boardRev = 1 ;
     else
     boardRev = 2 ;*/
  boardRev = 1 ;

  return boardRev ;
}

/*
 * pwmSetMode:
 *Select the native "balanced" mode, or standard mark:space mode
 *********************************************************************************
 */

void pwmSetMode (int p_mode)
{
  // Nothing to do here, just for compatibility with WiringPI PWM module
} // End of function pwmSetMode

/*
 * pwmSetRange:
 *Set the PWM range register. We set both range registers to the same
 *value. If you want different in your own code, then write your own.
http://raspberrypi.stackexchange.com/questions/4906/control-hardware-pwm-frequency
 *********************************************************************************
 */

void pwmSetRange (int p_gpio, unsigned int p_range)
{
  // Sanity check
  if (sysFds[p_gpio] == NULL) {
    return;
  }

  sysFds[p_gpio]->types.pwm.range = p_range;
  pwmUpdate(sysFds[p_gpio]);
} // End of function pwmSetRange

/*
 * pwmSetClock:
 *Set/Change the PWM clock. 
 *********************************************************************************
 */

void pwmSetClock (int p_gpio, unsigned int p_divisor)
{
  // Sanity check
  if (sysFds[p_gpio] == NULL) {
    return;
  }

  sysFds[p_gpio]->types.pwm.clock = p_divisor;
  pwmUpdate(sysFds[p_gpio]);
} // End of function pwmSetClock

void pwmUpdate (gpioDesc *p_gpio)
{
  char buffer[MAX_BUF];

  // Set the period
  printf("pwmUpdate: Divisor=%d - Clock=%d\n", p_gpio->types.pwm.range, p_gpio->types.pwm.clock);
  p_gpio->types.pwm.period = 1e9 / (PWM_FIXED_FREQUENCY / p_gpio->types.pwm.clock / p_gpio->types.pwm.range);
  sprintf(buffer, "%d", (unsigned int)p_gpio->types.pwm.period);
  printf("pwmUpdate: Set period to %s\n", buffer);
  lseek(p_gpio->types.pwm.fdp, 0L, SEEK_SET) ;
  write(p_gpio->types.pwm.fdp, buffer, MAX_BUF);
  /*sprintf(buffer, "echo %d > %s%s/period", (unsigned int)p_gpio->types.pwm.period, SYSFS_PWM_DIR, p_gpio->types.pwm.pwmChip);
  printf("pwmUpdate: call system with %s\n", buffer);
  system(buffer);*/
} // End of function pwmUpdate

const char * pwmChipFromPin(const unsigned int p_gpio) {
  printf("pwmChipFromPin: %d\n", p_gpio);
  switch (p_gpio) { // FIXME Check /sys/devices/ocp.3/pwm_test_Px.yy to check the correct extention
  case 3: // p9.21 - gpio0_03
    return NULL;
  case 2: // p9.22 - gpio0_02
    return NULL;
  case 22: // P8.19 - gpio0_22
    return "P8_19.13";
  case 23: // P8.13 - gpio0_23
    return NULL;
  case 50: // P9.14 - gpio1_18
    return "P9_14.12";
  case 51: // p9.16 - gpio1_19
    return "P9_16.16";
  } // End of 'switch' statement

  return NULL;
} // End of function pwmChipFromPin

/*
 * pinMode:
 * Sets the mode of a pin to be input, output or PWM output
 *********************************************************************************
 */

void pinMode (int p_gpio, int p_mode)
{
  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    return;
  }

  if (sysFds[p_gpio] == NULL) {
    switch (p_mode) {
    case INPUT:
      // No break;
    case OUTPUT:
      // Allocation GPIO
      if (allocGpio(p_gpio, Weak, Digital) < 0) {
	return;
      }
      break;
    case PWM_OUTPUT:
      // Allocation GPIO
      if (allocGpio(p_gpio, Weak, Pwm) < 0) {
	  return;
      }
      break;
    } // End of 'switch' statement
  }

  // Set the direction
  switch (p_mode) {
  case INPUT:
    // Set INPUT mode
    write(sysFds[p_gpio]->types.digital.fdd, "in", 2);
    break;
  case OUTPUT:
    // Set INPUT mode
    write(sysFds[p_gpio]->types.digital.fdd, "out", 3);
    break;
  case PWM_OUTPUT:
    // Set duty cycle
    pwmWrite(p_gpio, 50);
    // Start PWM
    write(sysFds[p_gpio]->types.pwm.fdr, "1", 1);
    break;
  } // End of 'switch' statement
} // End of function pinMode



/*
 * pullUpDownCtrl:
 *Control the internal pull-up/down resistors on a GPIO pin
 *The Arduino only has pull-ups and these are enabled by writing 1
 *to a port when in input mode - this paradigm doesn't quite apply
 *here though.
 *********************************************************************************
 */
void pullUpDnControl(int p_gpio, int p_pud) {
  // FIXME How to dynamically modify Pull Up/Down without modifying Device Tree Overlay 
}



/*
 * digitalRead:
 * Read the value of a given Pin, returning HIGH or LOW
 *********************************************************************************
 */

int digitalRead (int p_gpio)
{
  char c;

  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    return -1;
  }
  
  // On-Board Pin
  if (sysFds[p_gpio] == NULL) {
    // Allocation GPIO
    if (allocGpio(p_gpio, Weak, Digital) < 0) {
      return LOW;
    }
    // Set direction in
    pinMode(p_gpio, INPUT);
  }
  // Read value
  lseek(sysFds[p_gpio]->types.digital.fd, 0L, SEEK_SET) ;
  read(sysFds[p_gpio]->types.digital.fd, &c, 1) ;
  return (c == '0') ? LOW : HIGH;
}

/*
 * digitalWrite:
 * Set an output bit
 *********************************************************************************
 */

void digitalWrite (int p_gpio, int p_value)
{
  //  printf(">>> digitalWrite: %d, %d\n", p_gpio, p_value);
  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    fprintf(stderr, "digitalWrite: Wrong parameter\n");
    return;
  }

  if (sysFds[p_gpio] == NULL) {
    // Allocation GPIO
    if (allocGpio(p_gpio, Weak, Digital) < 0) {
      return;
    }
    // Set direction out
    pinMode(p_gpio, OUTPUT);
  }
  // Write value
  lseek(sysFds[p_gpio]->types.digital.fd, 0, SEEK_SET);
  if (p_value == LOW)
    write(sysFds[p_gpio]->types.digital.fd, "0", 1) ;
  else
    write(sysFds[p_gpio]->types.digital.fd, "1", 1) ;
  
  //  printf("<<< digitalWrite\n");
  return;
}

/*
 * pwmWrite:
 *Set an output PWM value
 *********************************************************************************
 */

void pwmWrite (int p_gpio, unsigned int p_value)
{
  char buffer[MAX_BUF];

  // Sanity check
  if (sysFds[p_gpio] == NULL) {
    // FIXME Add default behavior
    return;
  }

  // Set the duty_cycle
  sysFds[p_gpio]->types.pwm.duty_cycle = sysFds[p_gpio]->types.pwm.period * (100 - p_value) / 100;
  sprintf(buffer, "%d", (unsigned int)sysFds[p_gpio]->types.pwm.duty_cycle);
  printf("Set duty cycle to %s/%d\n", buffer, (unsigned int)sysFds[p_gpio]->types.pwm.period);
  lseek(sysFds[p_gpio]->types.pwm.fddc, 0L, SEEK_SET) ;
  write(sysFds[p_gpio]->types.pwm.fddc, buffer, MAX_BUF);
} // End of function pwmWrite



/*
 * interruptHandler:
 *This is a thread and gets started to wait for the interrupt we're
 *hoping to catch. It will call the user-function when the interrupt
 *fires.
 *********************************************************************************
 */

static void *interruptHandler (void * p_arg)
{
  int result;

  for (;;) {
    if (isrFdsNum == 0) {
      printf("interruptHandler: No GPIOs to poll: terminate thread\n");
      break;
    }
    result = poll(isrFds, isrFdsNum + 1, POLL_TIMEOUT); 
    if (result == -1) {
      fprintf(stderr, "interruptHandler: select call failed: %s\n", strerror(errno));
      break;
    } else if (result != 0) { // Check on which GPIO the interrupt occurs
      //      printf("interruptHandler: Something happening: %d\n", result);
      int i = 0;
      for ( ; i < isrFdsNum; i++) {
	if (isrFds[i].revents & POLLIN) {
	  unsigned char c;

	  //	  printf("interruptHandler: isr on gpio %d\n", sysFds[isrGpios[i]]->gpio);
	  read(sysFds[isrGpios[i]]->types.digital.fd, &c, 1); // Clear ISR
	  isrFds[i].revents = 0;
	  sysFds[isrGpios[i]]->types.digital.callback.callback_fn(sysFds[isrGpios[i]]->types.digital.callback.callback_arg);	  
	}
      } // End of 'for' statement
    } // No ISR, nothing to do
  } // End of 'for' statement

  memset((void *)&isrFds, 0x00, sizeof(isrFds));
  isrFdsNum = 0;
  pthread_exit(NULL); // Terminate thread
  return NULL ;
}

/*
 * wiringBbbISR:
 * BBB Specific.
 *Take the details and create an interrupt handler that will do a call-
 *back to the user supplied function.
 *********************************************************************************
 */

int wiringBbbISR (int p_gpio, int p_mode, int (*p_function)(void *))
{
  const char *modeS;
  char buffer[MAX_BUF];
  int flags;

  // Sanity check
  if (p_gpio > MAX_GPIO_ID) {
    fprintf(stderr, "wiringBbbISR: Wrong parameter\n");
    return -1;
  } 
  // p_function could be null

  if (sysFds[p_gpio] == NULL) {
    // Allocation GPIO
    if (allocGpio(p_gpio, Weak, Digital) < 0) {
      return -2;
    }
    // Set direction in
    pinMode(p_gpio, INPUT);    
  }

  // Set mode
  if (sysFds[p_gpio]->types.digital.fdm == -1) { // Open mode file descriptor
    sprintf (buffer, "%s/gpio%d/edge", SYSFS_GPIO_DIR, p_gpio);
    printf("wiringBbbISR: open '%s'\n", buffer);
    sysFds[p_gpio]->types.digital.fdm = open(buffer, O_RDWR);
  } // File descriptor is opened
  if (p_mode != INT_EDGE_SETUP) {
    if (p_mode == INT_EDGE_FALLING)
      modeS = "falling" ;
    else if (p_mode == INT_EDGE_RISING)
      modeS = "rising" ;
    else
      modeS = "both" ;
    printf("wiringBbbISR: mode '%s'\n", modeS);
    write(sysFds[p_gpio]->types.digital.fdm, modeS, strlen(modeS) + 1);
  }

  // Set data structures
  sysFds[p_gpio]->types.digital.callback.callback_fn = p_function;
  sysFds[p_gpio]->types.digital.callback.ready = isrFdsNum;
  sysFds[p_gpio]->types.digital.callback.callback_arg = (void *)sysFds[p_gpio];
  // Add O_NONBLOCK to the file descriptor flags 
  flags = fcntl(sysFds[p_gpio]->types.digital.fd, F_GETFL, 0);
  if (fcntl(sysFds[p_gpio]->types.digital.fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    fprintf(stderr, "wiringBbbISR: fcntl call failed: %s", strerror(errno));
  }
  // TODO Block execution here 
  isrFds[isrFdsNum].fd = sysFds[p_gpio]->types.digital.fd;
  isrFds[isrFdsNum].events = POLLIN | POLLPRI | POLLHUP;
  isrFds[isrFdsNum].revents = 0;
  isrGpios[isrFdsNum] = p_gpio;
  isrFdsNum += 1;
  // Start thread
  if (isrThreadId == -1) { // Create the ISR thread
    printf("wiringBbbISR: Create thread\n");
    pthread_create(&isrThreadId, NULL, interruptHandler, NULL);
  }
  // TODO Unblock execution here

  return 0;
} // End of function wiringBbbISR

/*
 * initialiseEpoch:
 *	Initialise our start-of-time variable to be the current unix
 *	time in milliseconds and microseconds.
 *********************************************************************************
 */
static void initialiseEpoch (void)
{
  struct timeval tv ;

  gettimeofday (&tv, NULL) ;
  epochMilli = (uint64_t)tv.tv_sec * (uint64_t)1000    + (uint64_t)(tv.tv_usec / 1000) ;
  epochMicro = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec) ;
}


/*
 * delay:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */

void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}


/*
 * delayMicroseconds:
 *	This is somewhat intersting. It seems that on the BBB, a single call
 *	to nanosleep takes some 80 to 130 microseconds anyway, so while
 *	obeying the standards (may take longer), it's not always what we
 *	want!
 *
 *	So what I'll do now is if the delay is less than 100uS we'll do it
 *	in a hard loop, watching a built-in counter on the ARM chip. This is
 *	somewhat sub-optimal in that it uses 100% CPU, something not an issue
 *	in a microcontroller, but under a multi-tasking, multi-user OS, it's
 *	wastefull, however we've no real choice )-:
 *
 *      Plan B: It seems all might not be well with that plan, so changing it
 *      to use gettimeofday () and poll on that instead...
 *********************************************************************************
 */

void delayMicrosecondsHard (unsigned int howLong)
{
  struct timeval tNow, tLong, tEnd ;

  gettimeofday (&tNow, NULL) ;
  tLong.tv_sec  = howLong / 1000000 ;
  tLong.tv_usec = howLong % 1000000 ;
  timeradd (&tNow, &tLong, &tEnd) ;

  while (timercmp (&tNow, &tEnd, <))
    gettimeofday (&tNow, NULL) ;
}

void delayMicroseconds (unsigned int howLong)
{
  struct timespec sleeper ;
  unsigned int uSecs = howLong % 1000000 ;
  unsigned int wSecs = howLong / 1000000 ;

  /**/ if (howLong ==   0)
    return ;
  else if (howLong  < 100)
    delayMicrosecondsHard (howLong) ;
  else
    {
      sleeper.tv_sec  = wSecs ;
      sleeper.tv_nsec = (long)(uSecs * 1000L) ;
      nanosleep (&sleeper, NULL) ;
    }
}


/*
 * millis:
 *	Return a number of milliseconds as an unsigned int.
 *********************************************************************************
 */

unsigned int millis (void)
{
  struct timeval tv ;
  uint64_t now ;

  gettimeofday (&tv, NULL) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;

  return (uint32_t)(now - epochMilli) ;
}


/*
 * micros:
 *	Return a number of microseconds as an unsigned int.
 *********************************************************************************
 */

unsigned int micros (void)
{
  struct timeval tv ;
  uint64_t now ;

  gettimeofday (&tv, NULL) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec ;

  return (uint32_t)(now - epochMicro) ;
}

/*
 * wiringPiSetup:
 *Must be called once at the start of your program execution.
 *
 * Default setup: Initialises the system into wiringPi Pin mode and uses the
 *memory mapped hardware directly.
 *********************************************************************************
 */
int wiringBbbSetup (void)
{
  return wiringBbbSetupSys();
}

/*
 * wiringBbbSetupSys:
 *	Must be called once at the start of your program execution.
 *
 * Initialisation (again), however this time we are using the /sys/class/gpio
 *	interface to the GPIO systems - slightly slower, but always usable as
 *	a non-root user, assuming the devices are already exported and setup correctly.
 */
int wiringBbbSetupSys (void)
{
  if(isInit == FALSE) {
    int boardRev;
    
    // Configure table according to board version
    boardRev = bbbBoardRev();
    if (boardRev == 1) {
    } // else, handle errors
    
    initialiseEpoch() ;
    
    // Initalize sysFds table
    memset((void *)sysFds, 0x00, MAX_GPIO_ID * sizeof(gpioDesc *));
    
    memset((void *)&isrFds, 0x00, sizeof(isrFds));
    isrFdsNum = 0;
    
    isInit = TRUE;
  }

  return 0;
}

void wiringBbbShutdownSys (void)
{
  printf(">>> wiringBbbShutdownSys\n");

  int i = 0;

  for(i = 0; i < MAX_GPIO_ID; i++) {
    if (sysFds[i] != NULL) {
      freeGpio(sysFds[i]->gpio);
    }
  } // End of 'for' statement

  return;
}

int allocGpio(const unsigned int p_gpio, const gpioMode p_gpioMode, const gpioType p_gpioType) {
  gpioDesc *gpio;
  char buffer[MAX_BUF];

  // Sanity checks
  if (p_gpio > MAX_GPIO_ID) {
    return -1;
  }

  if (sysFds[p_gpio] == NULL) {
    int fd; // Used for export
    
    // Allocate value
    gpio = (gpioDesc *)malloc(sizeof(gpioDesc));
    gpio->gpio = p_gpio;
    gpio->shared = 0;
    // Export it 
    switch (p_gpioType) {
    case Digital:
      gpio->type = Digital;
      gpio->types.digital.callback.callback_fn = NULL;
      gpio->types.digital.callback.callback_arg = NULL;
      gpio->types.digital.callback.ready = 0xff;
      sprintf(buffer, "%s/export", SYSFS_GPIO_DIR);
      if ((fd = open(buffer, O_SYNC | O_WRONLY)) < 0) {
	free(gpio);
	return -1;
      }
      sprintf(buffer, "%d", p_gpio);
      write(fd, buffer, MAX_BUF);
      close(fd);
      // Open gpio/value file descriptor
      sprintf (buffer, "%s/gpio%d/value", SYSFS_GPIO_DIR, p_gpio) ;
      gpio->types.digital.fd = open(buffer, O_RDWR);
      sprintf (buffer, "%s/gpio%d/direction", SYSFS_GPIO_DIR, p_gpio) ;
      gpio->types.digital.fdd = open(buffer, O_RDWR);
      gpio->types.digital.fdm = -1;
      break;
    case Pwm:
      gpio->type = Pwm;
      gpio->types.pwm.pwmChip = pwmChipFromPin(p_gpio);
      sprintf(buffer, "%s%s/run", SYSFS_PWM_DIR, gpio->types.pwm.pwmChip);
      if ((gpio->types.pwm.fdr = open(buffer, O_SYNC | O_WRONLY)) < 0) {
	free(gpio);
	return -1;
      }
      write(gpio->types.pwm.fdr, "0", 1);
      sprintf(buffer, "%s%s/period", SYSFS_PWM_DIR, gpio->types.pwm.pwmChip);
      gpio->types.pwm.fdp = open(buffer, O_SYNC | O_WRONLY);
      sprintf(buffer, "%s%s/duty", SYSFS_PWM_DIR, gpio->types.pwm.pwmChip);
      gpio->types.pwm.fddc = open(buffer, O_SYNC | O_WRONLY);
      gpio->types.pwm.range = 256;
      gpio->types.pwm.clock = 1;
      pwmUpdate(gpio);
      break;
    case Adc:
      break;
    } // End of 'switch' mode
    sysFds[p_gpio] = gpio;
  } else if (p_gpioMode == Weak) {
    free(gpio);
    return -1; // TODO Error
  } else {
    gpio = sysFds[p_gpio];
    if (p_gpioMode == Shared) {
      gpio->shared += 1;
    }
  }
  
  return 0;
}

int freeGpio(const unsigned int p_gpio) {
  gpioDesc * gpio;
  
  // Sanity checks
  if ((p_gpio > MAX_GPIO_ID) || (sysFds[p_gpio] == NULL)) {
    return -1;
  }

  gpio = sysFds[p_gpio];
  if (gpio->shared == 0) { // Free it
    int fd; // Used for unexport
    char buffer[MAX_BUF];

    switch (gpio->type) {
    case Digital:
      // Close file descriptors
      close(gpio->types.digital.fd);
      close(gpio->types.digital.fdd);
      if (gpio->types.digital.fdm != -1) {
	close(gpio->types.digital.fdm);
      }
      if (gpio->types.digital.callback.ready != 0xff) {
	gpio->types.digital.callback.ready = 0xff;
	isrFds[gpio->types.digital.callback.ready].fd = -1;
	isrFdsNum -= 1;
      }
      // Unexport the GPIO
      sprintf(buffer, "%s/unexport", SYSFS_GPIO_DIR);
      if ((fd = open(buffer, O_SYNC | O_WRONLY)) >= 0) {
	sprintf(buffer, "%d", p_gpio);
	write(fd, buffer, MAX_BUF);
	close(fd);
      }
      break;
    case Pwm:
      // Disable the PWM
      lseek(gpio->types.pwm.fdr, 0L, SEEK_SET) ;
      write(gpio->types.pwm.fdr, "0", 1);
      // Close file descriptors
      close(gpio->types.pwm.fdr);
      close(gpio->types.pwm.fdp);
      close(gpio->types.pwm.fddc);
      break;
    case Adc:
      // FIXME TO be done
      break;
    } // End of 'switch' statement
    // Free the GPIO descriptor
    free((void *)gpio);
    sysFds[p_gpio] = NULL;
  } else {
    gpio->shared -= 1;
  }
  
  return 0;
}
