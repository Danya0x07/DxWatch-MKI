#include "embedded_common.h"
#include <buttons.h>
#include <builtin_led.h>
#include <pwrlatch.h>

void Global_Setup(void)
{
    Button0_DisableInterrupt();
    PWRLATCH_ON();
}

void Global_TearDown(void)
{
    PWRLATCH_OFF();
}

void InfiniteLoop(void) {}

void setUp(void) {}

void tearDown(void)
{
    blink(7, 100);
}

void Test_EventPolling(void)
{
    uint32_t now = HAL_GetTick();
    ButtonEvent_t event;

    while (HAL_GetTick() - now < 10000) {
        for (uint8_t btn = BTN0; btn <= BTN3; btn++) {
            event = Button_GetEvent(btn);
            if (event == ButtonEvent_PRESS)
                blink(1, 200);
            else if (event == ButtonEvent_RELEASE)
                blink(2, 200);
        }
    }
}

void Test_Interrupt(void)
{
    for (int i = 1; i < 3; i++) {
        Button0_EnableInterrupt();
        HAL_Delay(3000);
        Button0_DisableInterrupt();
        HAL_Delay(3000);
    }
}

EXPORT_TEST_CASES(Test_EventPolling, Test_Interrupt);