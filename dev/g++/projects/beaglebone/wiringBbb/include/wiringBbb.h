

#ifndef	__WIRING_BBB_H__
#define	__WIRING_BBB_H__

// Handy defines
#define	WBBB_MODE_PINS		 0
#define	WBBB_MODE_GPIO		 1
#define	WBBB_MODE_GPIO_SYS	 2
#define	WBBB_MODE_PHYS		 3
#define	WBBB_MODE_PIFACE	 4
#define	WBBB_MODE_UNINITIALISED	-1

// Pin modes

#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3

#define	LOW			 0
#define	HIGH			 1

// Pull up/down/none

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

// PWM

#define	PWM_MODE_MS		0
#define	PWM_MODE_BAL		1

// Interrupt levels

#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3

// Threads

#define	PI_THREAD(X)	void *X (void *dummy)

// Failure modes

#define	WBBB_FATAL	(1==1)
#define	WBBB_ALMOST	(1==2)


// Function prototypes
//	c++ wrappers thanks to a comment by Nick Lott
//	(and others on the Raspberry Pi forums)

#ifdef __cplusplus
extern "C" {
#endif

  int wiringBbbFailure (int fatal, const char *message, ...);
extern int  wiringBbbSetup       (void) ;
extern int wiringBbbSetupSys      (void) ;
extern void wiringBbbShutdownSys  (void);
//extern int  wiringBbbSetupGpio   (void) ;
//extern int  wiringBbbSetupPhys   (void) ;

extern void pinMode             (int p_gpio, int p_mode) ;
extern void pullUpDnControl     (int p_gpio, int p_pud) ;
extern int  digitalRead         (int p_gpio) ;
extern void digitalWrite        (int p_gpio, int p_value) ;
extern void pwmWrite            (int p_gpio, unsigned int p_value) ;
extern int  analogRead          (int p_gpio) ;
extern void analogWrite         (int p_gpio, int p_value) ;

// On-Board BeagleBone Black hardware specific stuff

extern int  bbbBoardRev         (void) ;
extern void digitalWriteByte    (int p_value) ;
extern void pwmSetMode          (int p_mode) ;
extern void pwmSetRange         (int p_gpio, unsigned int p_range) ;
extern void pwmSetClock         (int p_gpio, unsigned int p_divisor) ;

// Interrupts
//	(Also BBB hardware specific)

extern int  waitForInterrupt    (int p_gpio, int p_mS) ;
extern int  wiringBbbISR        (int p_gpio, int p_mode, int (*p_function)(void *)) ;

// Threads

extern int  piThreadCreate      (void *(*fn)(void *)) ;
extern void piLock              (int key) ;
extern void piUnlock            (int key) ;

// Schedulling priority

extern int piHiPri (const int pri) ;

// Extras from arduino land

extern void         delay             (unsigned int howLong) ;
extern void         delayMicroseconds (unsigned int howLong) ;
extern unsigned int millis            (void) ;
extern unsigned int micros            (void) ;

#ifdef __cplusplus
}
#endif

#endif
