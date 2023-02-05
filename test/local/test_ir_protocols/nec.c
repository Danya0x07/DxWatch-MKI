#include <unity.h>
#include <string.h>
#include <ir_nec.h>

#define S   560
#define L   1690

const uint16_t stock_frame[100] = {
    9000, 4500, S, S, S, L, S, L, S, L,
    S, L, S, L, S, L, S, S, S, L,
    S, S, S, S, S, S, S, S, S, S,
    S, S, S, L, S, L, S, L, S, L,
    S, L, S, L, S, L, S, L, S, S,
    S, S, S, S, S, S, S, S, S, S,
    S, S, S, S, S, L, S, // 0
};

uint16_t frame[100];

void setUp()
{
    memcpy(frame, stock_frame, sizeof(frame));
}

void tearDown() {}

void Test_NEC_BadFrameBitsMismatch(void)
{
    struct NecPacket packet;
    NecParsingStatus_t status;
    
    frame[17] = L;
    status = NEC_ParseRaw(frame, 67, &packet);
    TEST_ASSERT_EQUAL(NecParsingStatus_BITS_MISMATCH, status);
}

void Test_NEC_BadFrameBadPulse(void)
{
    struct NecPacket packet;
    NecParsingStatus_t status;
    
    frame[10] = L;
    status = NEC_ParseRaw(frame, 67, &packet);
    TEST_ASSERT_EQUAL(NecParsingStatus_BAD_PULSE, status);
}

void Test_NEC_BadFrameBadLen(void)
{
    struct NecPacket packet;
    NecParsingStatus_t status;
    
    status = NEC_ParseRaw(frame, 65, &packet);
    TEST_ASSERT_EQUAL(NecParsingStatus_BAD_RAWLEN, status);
}

void Test_NEC_GoodFrame(void)
{
    struct NecPacket packet;
    NecParsingStatus_t status;
    
    status = NEC_ParseRaw(frame, 67, &packet);
    TEST_ASSERT_EQUAL(NecParsingStatus_OK, status);
    TEST_ASSERT_EQUAL(0x7E, packet.address);
    TEST_ASSERT_EQUAL(0x7F, packet.command);
}

void Test_NEC_BuildingFrame(void)
{
    struct NecPacket packet = {.address = 0x7E, .command = 0x7F};

    memset(frame, 0, sizeof(frame));
    NEC_BuildRaw(&packet, frame);
    TEST_ASSERT_EQUAL(0, memcmp(frame, stock_frame, sizeof(frame)));
}