#ifndef _RTC_H_
#define _RTC_H_

#include"stdutils.h"


/***************************************************************************************************
                                 Struct/Enums used
***************************************************************************************************/
typedef struct
{
  uint16_t min;
  uint16_t hour;
  uint16_t weekDay;
  uint16_t date;
  uint16_t month;
  uint16_t year;  
}rtc_t;
/**************************************************************************************************/


/***************************************************************************************************
                             Commonly used Ds1307 macros/Constants
***************************************************************************************************
  Below values are fixed and should not be changed. Refer Ds1307 DataSheet for more info*/

#define C_Ds1307ReadMode_U8   0xD1u  // DS1307 ID
#define C_Ds1307WriteMode_U8  0xD0u  // DS1307 ID

#define C_Ds1307SecondRegAddress_U8   0x00u   // Address to access Ds1307 SEC register
#define C_Ds1307DateRegAddress_U8     0x04u   // Address to access Ds1307 DATE register
#define C_Ds1307ControlRegAddress_U8  0x07u   // Address to access Ds1307 CONTROL register
/**************************************************************************************************/




/***************************************************************************************************
                             Function Prototypes
***************************************************************************************************/
void RTC_Init(void);
void RTC_SetDateTime(rtc_t *rtc);
void RTC_GetDateTime(rtc_t *rtc);
/**************************************************************************************************/

#endif
