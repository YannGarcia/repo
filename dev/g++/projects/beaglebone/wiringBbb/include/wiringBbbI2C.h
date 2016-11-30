#if !defined(__WIRINGBBB_I2C_H__)
#define __WIRINGBBB_I2C_H__

#ifdef __cplusplus
extern "C" {
  #endif

  extern int wiringBbbI2CRead           (int fd) ;
  extern int wiringBbbI2CReadReg8       (int fd, int reg) ;
  extern int wiringBbbI2CReadReg16      (int fd, int reg) ;

  extern int wiringBbbI2CWrite          (int fd, int data) ;
  extern int wiringBbbI2CWriteReg8      (int fd, int reg, int data) ;
  extern int wiringBbbI2CWriteReg16     (int fd, int reg, int data) ;

  extern int wiringBbbI2CSetupInterface (const char *device, int devId) ;
  extern int wiringBbbI2CSetup          (const int devId) ;

  #ifdef __cplusplus
}
#endif

#endif // __WIRINGBBB_I2C_H__
