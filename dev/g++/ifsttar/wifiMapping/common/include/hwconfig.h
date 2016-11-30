#if !defined(__HWCONFIG_H__)
#define __HWCONFIG_H__

#if defined(__RASPBERRY_PI_HW__)

/*****************************************************************************************
  Green/Red LEDs ports
 *****************************************************************************************/
#define LED_GREEN_PORT    12
#define LED_RED_PORT      13

/*****************************************************************************************
  UART ports
 *****************************************************************************************/
#define UART_TX    8
#define UART_RX    10

/*****************************************************************************************
  I2C ports
 *****************************************************************************************/
#define I2C_SDA    3
#define I2C_SCL    5

/*****************************************************************************************
  I2C device addresses
 *****************************************************************************************/
#define I2C_DS1307_ADDRESS   0x00
#define I2C_MCP79410_ADDRESS 0x00
#define I2C_MCP9801_ADDRESS  0x00 // <A2, A0> = 0 - See DS21909D-page 9 TABLE 3-2: SLAVE ADDRESS

/*****************************************************************************************
  SPI ports
 *****************************************************************************************/
#define SPI_SCK     23
#define SPI_MISO    21
#define SPI_MOSI    19
#define SPI_CS0_CAN 24
#define SPI_CS1_ADC 26

/*****************************************************************************************
  LCD ports
 *****************************************************************************************/
#define LCD_EN     16
#define LCD_RW     18
#define LCD_RS     15
#define LCD_DB0    0xff
#define LCD_DB1    0xff
#define LCD_DB2    0xff
#define LCD_DB3    0xff
#define LCD_DB4    0xf3
#define LCD_DB5    0xf4
#define LCD_DB6    0xf5
#define LCD_DB7    0xf5

#elif defined(__BEAGLE_BONE_BLACK_HW__)

/*****************************************************************************************
  Green/Red LEDs ports
 *****************************************************************************************/
#define LED_GREEN_PORT    44
#define LED_RED_PORT      23

/*****************************************************************************************
  UART ports
 *****************************************************************************************/
#define UART_TX    3
#define UART_RX    2

/*****************************************************************************************
  I2C ports
 *****************************************************************************************/
#define I2C_SDA    12
#define I2C_SCL    13

/*****************************************************************************************
  I2C device addresses
 *****************************************************************************************/
#define I2C_DS1307_ADDRESS   0x08
#define I2C_MCP79410_ADDRESS 0x00
#define I2C_MCP9801_ADDRESS  0x00 // <A2, A0> = 0 - See DS21909D-page 9 TABLE 3-2: SLAVE ADDRESS

/*****************************************************************************************
  SPI ports
 *****************************************************************************************/
#define SPI_SCK     110
#define SPI_MISO    111
#define SPI_MOSI    112
#define SPI_CS0_CAN 48
#define SPI_CS1_ADC 51

/*****************************************************************************************
  LCD ports
 *****************************************************************************************/
#define LCD_EN     46
#define LCD_RW     27
#define LCD_RS     65
#define LCD_DB0    0xff
#define LCD_DB1    0xff
#define LCD_DB2    0xff
#define LCD_DB3    0xff
#define LCD_DB4    75
#define LCD_DB5    74
#define LCD_DB6    77
#define LCD_DB7    76

/*****************************************************************************************
  FAN Control
 *****************************************************************************************/
#define FAN_PWM                50   // P9_24
#define FAN_ROTOR_LOCK_WARNING 61
#define FAN_TEMP_SHUTDOWN      40.0 // Celsius degrees

#else // Hardware selection

#error "No hardware flag defined"

#endif // Hardware selection

/*****************************************************************************************
  Vbutton channel port
 *****************************************************************************************/
#define ADC_CHANNEL_VBUTTON    0x07
#define RES_PUSHBUTTON_1       1.00  /** in kHom */
#define RES_PUSHBUTTON_2       4.71  /** in kHom */
#define RES_PUSHBUTTON_3       8.19  /** in kHom */
#define RES_PUSHBUTTON_4       21.7  /** in kHom */
#define RES_V_DIVIDER          32.6  /** in kHom */

#define V_PUSHBUTTON_1  2.428
#define V_PUSHBUTTON_2  2.187
#define V_PUSHBUTTON_3  2.001
#define V_PUSHBUTTON_4  1.500
#define V_PUSHBUTTON_12 2.441
#define V_DELTA         0.005

#define PUSHBUTTON_1_PUSHED  1
#define PUSHBUTTON_2_PUSHED  2
#define PUSHBUTTON_3_PUSHED  3
#define PUSHBUTTON_4_PUSHED  4
#define PUSHBUTTON_12_PUSHED 5
#define PUSHBUTTON_13_PUSHED 6
#define PUSHBUTTON_14_PUSHED 7

#endif // __HWCONFIG__

