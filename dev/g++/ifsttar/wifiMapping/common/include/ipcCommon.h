#if !defined(__IPC_COMMON_H__)
#define __IPC_COMMON_H__

#define SHAREDMEMORY_KEY            0xcafedeca
#define SHAREDMEMORY_SEGMENT_SIZE   512
#define SHAREDMEMORY_SEGMENT_NUMBER 8

/*****************************************************************************************
  Main application share memory settings
 *****************************************************************************************/
#define PARAMETER_SEGMENT_ID 0
#define PARAMETER_DATA_LENGTH   sizeof(float) /** _samplePeriod */

/*****************************************************************************************
  Iperf 3.x share memory settings
 *****************************************************************************************/
#define IPERF_SEGMENT_ID    1
#define IPERF_DATA_LENGTH   sizeof(unsigned char) + 2 * sizeof(float) /** xfer bandwidth */

/*****************************************************************************************
  GPS share memory settings
 *****************************************************************************************/
#define GPS_SEGMENT_ID   2 /** UTC dae/time of position, lat, long, speed, altitude, heading, yawrate */

#define IDX_UTC_DATETIME 0 /** UTC date/time of position */
#define IDX_LATITUDE     1 /** Latitude: <E|S>xxmm.dddd + NULL */
#define IDX_LONGITUDE    2 /** Longitude: <E|W>yyymm.dddd + NULL */
#define IDX_SPEED        3 /** Speed: s.s + NULL */
#define IDX_ALTITUDE     4 /** Altitude: s.s + NULL */
#define IDX_HEADING      5 /** Heading: s.s + NULL */
#define IDX_YAWRATE      6 /** YawRate: s.s + NULL */

#define GPS_DATA_LENGTH     sizeof(unsigned char) + 17 + 11 + 12 + 8 + 8 + 8 + 8
                                
/*****************************************************************************************
  Acc share memory settings
 *****************************************************************************************/
#define ACC_SEGMENT_ID      3
#define ACC_DATA_LENGTH     sizeof(unsigned char) + 3 * sizeof(float) /** Gx Gy Gz */

/*****************************************************************************************
  Temp share memory settings
 *****************************************************************************************/
#define TEMP_SEGMENT_ID     4
#define TEMP_DATA_LENGTH    sizeof(unsigned char) + sizeof(float) /** Temperature */

/*****************************************************************************************
  RTC share memory settings
 *****************************************************************************************/
#define RTC_SEGMENT_ID     5
#define RTC_DATA_LENGTH    sizeof(unsigned char) + sizeof(time_t) /** RTC status + time */

/*****************************************************************************************
  Temp share memory settings
 *****************************************************************************************/
#define WLAN_SEGMENT_ID     6
#define WLAN_DATA_LENGTH    256 /** BSSID + Data rate + Power + Channel + ESSID  */

#define IDX_BSSID    0
#define IDX_DATARATE 1
#define IDX_RSSID    2
#define IDX_CHANNEL  3
#define IDX_ESSID    4

/*****************************************************************************************
  Message queue setting
 *****************************************************************************************/
#define MESSAGEQUEUE_KEY   0xdeadbeef

/*****************************************************************************************
  Data storage message queue setting
 *****************************************************************************************/
#define DS_MQ_SIZE         (IPERF_DATA_LENGTH + GPS_DATA_LENGTH + ACC_DATA_LENGTH + TEMP_DATA_LENGTH + RTC_DATA_LENGTH + WLAN_DATA_LENGTH)

/*****************************************************************************************
  GUI message queue setting
 *****************************************************************************************/
#define GUI_MQ_SIZE        256

#define GUI_DEVICE_LCD_ID 0x00
#define GUI_DEVICE_ADC_ID 0x01
#define GUI_DEVICE_PHB_ID 0x02


#endif // __IPC_COMMON_H__

