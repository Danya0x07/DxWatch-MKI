#ifndef INC_EMBEDDED_COMMON
#define INC_EMBEDDED_COMMON

#include <unity.h>
#include <main.h>

#define EXPORT_TEST_CASES(...) \
void (*const TEST_CASES[])(void) = { \
    __VA_ARGS__ \
}; \
const int NUM_OF_TEST_CASES = sizeof(TEST_CASES) / sizeof(TEST_CASES[0])

#endif // INC_EMBEDDED_COMMON