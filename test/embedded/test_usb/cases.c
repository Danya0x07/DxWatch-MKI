#include "embedded_common.h"
#include <usb.h>
#include <usbd_cdc_if.h>
#include <builtin_led.h>

void Global_Setup(void)
{
    HAL_Delay(5000); // чтобы просралось
}

void Global_TearDown(void) {}

void InfiniteLoop(void) {}

void setUp(void) {}

void tearDown(void)
{
    blink(7, 100);
}

void Test_SendString(void)
{
    const char *str = "Hello basstards!";
    for (int i = 0; i < 8; i++) {
        blink(1, 200);
        CDC_Transmit_FS((uint8_t *)str, strlen(str));
        HAL_Delay(1000);
    }
}

void Test_ReceiveString(void)
{
    
    for (int i = 0; i < 8; i++) {
        uint32_t now = HAL_GetTick();
        while (HAL_GetTick() - now < 2000) {
            if (testVariable) {
                blink(testVariable, 300);
                testVariable = 0;
            }
        }
        if (__NVIC_GetEnableIRQ(USB_IRQn)) {
            HAL_NVIC_DisableIRQ(USB_IRQn);
        } else {
            HAL_NVIC_EnableIRQ(USB_IRQn);
        }
        blink(1, 100);
    }
}

EXPORT_TEST_CASES(Test_SendString, Test_ReceiveString);