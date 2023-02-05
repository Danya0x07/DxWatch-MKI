#include <unity.h>

extern void Test_NEC_BadFrameBitsMismatch(void),
            Test_NEC_BadFrameBadPulse(void),
            Test_NEC_BadFrameBadLen(void),
            Test_NEC_GoodFrame(void),
            Test_NEC_BuildingFrame(void);

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_NEC_BadFrameBitsMismatch);
    RUN_TEST(Test_NEC_BadFrameBadPulse);
    RUN_TEST(Test_NEC_BadFrameBadLen);
    RUN_TEST(Test_NEC_GoodFrame);
    RUN_TEST(Test_NEC_BuildingFrame);
    return UNITY_END();
}