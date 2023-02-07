#include "embedded_common.h"
#include <adc.h>
#include <usart.h>
#include <builtin_led.h>
#include <stdlib.h>

volatile bool testFlag = false;
static char strbuff[32];

void Global_Setup(void)
{
    HAL_ADCEx_Calibration_Start(&hadc);
}

void Global_TearDown(void) {}

void InfiniteLoop(void) {}

void setUp(void) {}

void tearDown(void) {}

void Test_Measuring(void)
{
    uint16_t voltage;
    int16_t temperature;
    
    testFlag = false;
    ADC_PerformMeasurements();
    while (!testFlag) {
        BUILTIN_LED_TOGGLE();
        HAL_Delay(100);
    }
    voltage = ADC_ReadSupplyVoltage();
    temperature = ADC_ReadJunctionTemperature();
    UART_SendString("\n\nVoltage: ");
    UART_SendString(itoa(voltage, strbuff, 10));
    UART_SendString("\tTemperature: ");
    UART_SendString(itoa(temperature, strbuff, 10));
    HAL_Delay(3000);
}

EXPORT_TEST_CASES(Test_Measuring, Test_Measuring, Test_Measuring, Test_Measuring, Test_Measuring);