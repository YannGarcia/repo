
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "wiringBbb.h"

#include "wiringBbbSPI.h"


// The SPI bus parameters
//Variables as they need to be passed as pointers later on

const static char        *spiDev0  = "/dev/spidev1.0" ;
const static char        *spiDev1  = "/dev/spidev1.1" ;
static uint8_t           spiMode [2] ;
static uint8_t           spiBPW  [2] ;
const static uint16_t    spiDelay = 0 ;

static uint32_t    spiSpeeds [2] ;
static int         spiFds [2] ;


/*
 * wiringBbbSPIGetFd:
 *Return the file-descriptor for the given channel
 *********************************************************************************
 */

int wiringBbbSPIGetFd (int channel)
{
  return spiFds [channel & 1] ;
}


/*
 * wiringBbbSPIDataRW:
 *Write and Read a block of data over the SPI bus.
 *Note the data ia being read into the transmit buffer, so will
 *overwrite it!
 *This is also a full-duplex operation.
 *********************************************************************************
 */

int wiringBbbSPIDataRW (int channel, unsigned char *data, int len)
{
  struct spi_ioc_transfer spi ;

  channel &= 1 ;

  spi.tx_buf        = (unsigned long)data ;
  spi.rx_buf        = (unsigned long)data ;
  spi.len           = len ;
  spi.delay_usecs   = spiDelay ;
  spi.speed_hz      = spiSpeeds [channel] ;
  spi.bits_per_word = spiBPW [channel] ;

  return ioctl (spiFds [channel], SPI_IOC_MESSAGE(1), &spi) ;
}


/*
 * wiringBbbSPISetup:
 *Open the SPI device, and set it up, etc.
 *********************************************************************************
 */

int wiringBbbSPISetup (int channel, int speed)
{
  int fd ;

  channel &= 1 ;

  if ((fd = open (channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0)
    return wiringBbbFailure (WBBB_ALMOST, "Unable to open SPI device: %s\n", strerror (errno)) ;

  spiSpeeds [channel] = speed ;
  spiFds    [channel] = fd ;
  spiMode[channel]    = 0; // Mode 0,0

  // Set SPI parameters.
  //Why are we reading it afterwriting it? I've no idea, but for now I'm blindly
  //copying example code I've seen online...

  if (ioctl (fd, SPI_IOC_WR_MODE, &spiMode[channel])         < 0)
    return wiringBbbFailure (WBBB_ALMOST, "SPI Mode Change failure: %s\n", strerror (errno)) ;

  if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW[channel]) < 0)
    return wiringBbbFailure (WBBB_ALMOST, "SPI BPW Change failure: %s\n", strerror (errno)) ;

  if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)            < 0)
    return wiringBbbFailure (WBBB_ALMOST, "SPI Speed Change failure: %s\n", strerror (errno)) ;

  return fd ;
}

int wiringBbbSPIFormat(int channel, uint8_t bpw, uint8_t mode)
{
  spiMode   [channel] = mode;
  spiBPW    [channel] = bpw;

  if (ioctl (spiFds[channel], SPI_IOC_WR_MODE, &spiMode[channel])         < 0)
    return wiringBbbFailure (WBBB_ALMOST, "SPI Mode Change failure: %s\n", strerror (errno)) ;

  if (ioctl (spiFds[channel], SPI_IOC_WR_BITS_PER_WORD, &spiBPW[channel]) < 0)
    return wiringBbbFailure (WBBB_ALMOST, "SPI BPW Change failure: %s\n", strerror (errno)) ;

  return 0;
}
