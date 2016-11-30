#if !defined(__WIRINGBBB_SPI_H__)
#define __WIRINGBBB_SPI_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
  #endif

  int wiringBbbSPIGetFd  (int channel) ;
  int wiringBbbSPIDataRW (int channel, unsigned char *data, int len) ;
  int wiringBbbSPISetup  (int channel, int speed) ;
  int wiringBbbSPIFormat  (int channel, uint8_t bpw, uint8_t mode) ;

  #ifdef __cplusplus
}
#endif

#endif // __WIRINGBBB_SPI_H__
