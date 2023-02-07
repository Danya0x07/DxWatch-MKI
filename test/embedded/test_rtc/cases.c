#include "embedded_common.h"
#include <rtc.h>
#include <builtin_led.h>

void Global_Setup(void) {}

void Global_TearDown(void) {}

void InfiniteLoop(void) {}

void setUp(void) {}

void tearDown(void)
{
    HAL_Delay(1000);
}

void Test_TimeGoing(void)
{
    LL_RTC_DateTypeDef date;
    LL_RTC_TimeTypeDef time;
    uint8_t bkpSeconds;
    RTC_GetDateTime(&date, &time);
    bkpSeconds = time.Seconds;
    HAL_Delay(1100);
    RTC_GetDateTime(&date, &time);
    if (time.Seconds == bkpSeconds + 1) {
        blink(1, 100);
    }
}

void Test_DateGoing(void)
{
    LL_RTC_DateTypeDef date = {
        .Day = 31,
        .WeekDay = LL_RTC_WEEKDAY_TUESDAY,
        .Month = LL_RTC_MONTH_MARCH,
        .Year = 23
    };
    LL_RTC_TimeTypeDef time = {
        .Hours = 23,
        .Minutes = 59,
        .Seconds = 59
    };
    RTC_SetDateTime(&date, &time);
    HAL_Delay(1100);
    RTC_GetDateTime(&date, &time);
    if (
        time.Seconds == 0 && time.Minutes == 0 && time.Hours == 0
        //date.Day == 1 && date.WeekDay == LL_RTC_WEEKDAY_WEDNESDAY &&
        //date.Month == LL_RTC_MONTH_APRIL && date.Year == 23
    ) {
        blink(2, 100);
    }
}

void Test_AlarmWorking(void)
{
    LL_RTC_DateTypeDef date = {
        .Day = 31,
        .WeekDay = LL_RTC_WEEKDAY_TUESDAY,
        .Month = LL_RTC_MONTH_MARCH,
        .Year = 23
    };
    LL_RTC_TimeTypeDef time = {
        .Hours = 9,
        .Minutes = 0,
        .Seconds = 0
    };
    bool good = true;
    RTC_SetDateTime(&date, &time);
    time.Seconds = 3;
    RTC_ALARM_SetTime(&time);
    RTC_ALARM_Enable();
    good &= !BUILTIN_LED_IS_ON();
    while (!BUILTIN_LED_IS_ON())
        HAL_Delay(50);
    RTC_GetDateTime(&date, &time);
    good &= time.Seconds == 3;
    HAL_Delay(500);
    BUILTIN_LED_OFF();
    HAL_Delay(200);
    if (good) {
        blink(3, 100);
    }
}

EXPORT_TEST_CASES(
    Test_TimeGoing,
    Test_DateGoing,
    Test_AlarmWorking
);