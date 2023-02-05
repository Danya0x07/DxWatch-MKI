#include "embedded_common.h"
#include <ir_raw.h>
#include <tim.h>
#include <irtim.h>

#define BUFF_LEN    100
static uint16_t durations[BUFF_LEN];

void Global_Setup(void) {}

void Global_TearDown(void) {}

void InfiniteLoop(void) {}

void setUp(void){}

void tearDown(void) {}

void Test_RetransmitingFrame(void)
{
    int len = IR_RecordRaw(durations, BUFF_LEN, 5000, 20);

    switch (len) {
        case IR_NODATA:
            blink(1, 2000);
            break;
        
        case IR_OVERFLOW:
            blink(3, 200);
            break;
        
        case IR_BREAK:
            blink(1, 200);
            break;
        
        default:
            blink(2, 500);
            IRTIM_SetCarrierFrequency(38);
            IR_SendRaw(durations, len);
            break;
    }
}

#define S   560
#define L   1690

void Test_SendingFrame(void)
{
    const uint16_t frame[67] = {
        9000, 4500, S, S, S, L, S, L, S, L,
        S, L, S, L, S, L, S, S, S, L,
        S, S, S, S, S, S, S, S, S, S,
        S, S, S, L, S, L, S, L, S, L,
        S, L, S, L, S, L, S, L, S, S,
        S, S, S, S, S, S, S, S, S, S,
        S, S, S, S, S, L, S
    };
    IRTIM_SetCarrierFrequency(38);
    IR_SendRaw(frame, sizeof(frame) / sizeof(frame[0]));
}

EXPORT_TEST_CASES(
    Test_RetransmitingFrame,
    Test_RetransmitingFrame,
    Test_RetransmitingFrame,
    Test_RetransmitingFrame,
    Test_SendingFrame
);