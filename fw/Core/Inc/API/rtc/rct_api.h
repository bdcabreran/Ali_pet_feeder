#ifndef __RTC_API_H__
#define __RTC_API_H__

#include <stdint.h>

typedef struct date_time_st{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;
    uint16_t microseconds;
}date_time_t;

uint8_t rtc_init(uint8_t address);
void rtc_set_time(date_time_t date_time);

date_time_t rtc_get_time(void);
#endif /*__RTC_API_H__*/
