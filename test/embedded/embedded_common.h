#ifndef _EMBEDDED_COMMON_H
#define _EMBEDDED_COMMON_H

#include <unity.h>
#include <main.h>

#define EXPORT_TEST_CASES(...) \
void (*const testCases[])(void) = { \
    __VA_ARGS__ \
}; \
const int NUM_OF_TEST_CASES = sizeof(testCases) / sizeof(testCases[0])

void blink(int times, int duration);

#endif // _EMBEDDED_COMMON_H