#include <unity.h>

#include <main.h>
#include <adc.h>
#include <rtc.h>
#include <spi.h>
#include <tim.h>
#include <usart.h>
#include <usb.h>
#include <gpio.h>

extern void Global_Setup(void);
extern void Global_TearDown(void);
extern void InfiniteLoop(void);
extern void (*const testCases[])(void);
extern const int NUM_OF_TEST_CASES;

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14 | RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
    RCC_OscInitStruct.HSI14CalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_RTC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    
    MX_GPIO_Init();
    MX_RTC_Init();
    MX_TIM16_Init();
    MX_TIM17_Init();
    MX_ADC_Init();
    MX_SPI1_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_USB_PCD_Init();

    LL_SPI_Enable(SPI1);

    UNITY_BEGIN();
    Global_Setup();
    for (int i = 0; i < NUM_OF_TEST_CASES; i++) {
        if (testCases[i])
            RUN_TEST(testCases[i]);
    }
    Global_TearDown();
    UNITY_END();

    for (;;) {
        InfiniteLoop();
    }
}
