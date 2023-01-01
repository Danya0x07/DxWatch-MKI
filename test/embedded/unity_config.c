#include "unity_config.h"
#include <usart.h>

void Unity_OutputStart()
{
    MX_USART1_UART_Init();
}

void Unity_OutputChar(char c)
{
    UART_SendChar(c);
}

void Unity_OutputFlush() {}

void Unity_OutputComplete()
{
    LL_APB1_GRP2_DisableClock(LL_APB1_GRP2_PERIPH_USART1);
    LL_USART_Disable(USART1);
    LL_USART_DeInit(USART1);
}
