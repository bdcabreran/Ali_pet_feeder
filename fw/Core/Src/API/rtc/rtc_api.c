/**
 * @file rtc_api.c
 * @author Mauro R (emauriciorg@gmail.com)
 * @brief rtc HAL
 * @version 0.1
 * @date 2022-06-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdint.h>
#include "rct_api.h"
#include  "rtc_1307.h"
#include <string.h>
#include "printf_dbg.h"

void rtc_init_mock(void);
void rtc_set_time_mock(date_time_t date_time);
void rtc_get_time_mock(date_time_t *date_time);

date_time_t date_time_mock;

RTC_driver_t  mock_rtc_drv={
	rtc_init_mock,
	rtc_set_time_mock,
	rtc_get_time_mock
};


#ifdef RTC_MOCK
	RTC_driver_t  *rtc_drv = &mock_rtc_drv;
#else
	extern RTC_driver_t  *rtc_drv;
#endif



void rtc_init(uint8_t address){
	rtc_drv->init();
}

void rtc_set_time(date_time_t date_time){
	rtc_drv->set_time(date_time);
}

void rtc_get_time(date_time_t *date_time){
	rtc_drv->get_time(date_time);
}

void rtc_print_time(void)
{
	date_time_t date_time;
	rtc_get_time(&date_time);
	 printf("RTC Info : Date { DD/MM : %.2d/%.2d }, Time { hh:mm %.2d:%.2d %s }\r\n", \
	 date_time.dayOfMonth, date_time.month, date_time.hours, date_time.minutes, date_time.am_pm == RTC_TIME_AM ? "am" : "pm");
}
#ifdef RTC_MOCK
void rtc_init_mock(void){

}
void rtc_set_time_mock(date_time_t date_time){
	memcpy(&date_time_mock,&date_time,sizeof(date_time_t));
}
void rtc_get_time_mock(date_time_t *date_time){
	date_time_mock.seconds++;
	date_time_mock.minutes++;
	date_time_mock.hours++;
	memcpy(&date_time,&date_time_mock,sizeof(date_time_t));
}
#endif
