#include "embedded_common.h"
#include <tim.h>
#include <irtim.h>

void Global_Setup(void) {}

void Global_TearDown(void) {}

void InfiniteLoop(void) {}

void setUp(void) {}

void tearDown(void) {}

void Test_Carrier(void)
{
    uint8_t freqs[] = {36, 38, 40};

    for (int i = 0; i < sizeof(freqs) / sizeof(freqs[0]); i++) {
        IRTIM_SetCarrierFrequency(freqs[i]);
        IRTIM_EnableCarrier();
        IRTIM_SetLedState(1);
        HAL_Delay(10);
        IRTIM_SetLedState(0);
        IRTIM_DisableCarrier();
        HAL_Delay(50);
    }
}

EXPORT_TEST_CASES(Test_Carrier);