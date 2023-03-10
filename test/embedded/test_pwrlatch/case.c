#include "embedded_common.h"
#include <pwrlatch.h>
#include <builtin_led.h>
#include <flash_led.h>

static uint32_t now;

void Global_Setup(void)
{
    PWRLATCH_ON();
    FLASH_LED_ON();
    now = HAL_GetTick();
}

void Global_TearDown(void) {}

void InfiniteLoop(void) 
{
    BUILTIN_LED_TOGGLE();
    HAL_Delay(1000);
    if (HAL_GetTick() - now >= 5000 && PWRLATCH_IS_ON()) {
        PWRLATCH_OFF();
        FLASH_LED_OFF();
    }
}

void setUp(void) {}
void tearDown(void) {}

EXPORT_TEST_CASES(NULL);