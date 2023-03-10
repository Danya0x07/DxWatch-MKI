#include "os.h"
#include "buttons.h"
#include "adc.h"
#include "usbd_cdc_if.h"
#include "pwrlatch.h"
#include "display.h"
#include "builtin_led.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <queue.h>

#include <gfx.h>


TaskHandle_t taskButtonsEvents;
TaskHandle_t taskApplicationEventLoop;
TaskHandle_t taskTerminalService;

TimerHandle_t timerMeasureVoltage;
TimerHandle_t timerCustomRoutine;
TimerHandle_t timerShutdown;

#define SYSEVENT_QUEUE_LENGTH   10
QueueHandle_t queueSystemEvents;

void vApplicationIdleHook(void)
{
    // TODO
}

static void ReportError(const char *msg)
{
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, false);
    GFX_PrintString(msg);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    ReportError(pcTaskName);
    HAL_Delay(5000);
    PWRLATCH_OFF();
    for (;;) {
        BUILTIN_LED_TOGGLE();
        HAL_Delay(2000);
    }
}

void vApplicationMallocFailedHook(void)
{
    ReportError("M");
    for (;;) {
        BUILTIN_LED_TOGGLE();
        HAL_Delay(1000);
    }
}

void vAssertCalled(char *file, uint32_t line)
{
    ReportError(file);

    GFX_SetCursor(0, 1);
    GFX_PrintDec(line);
    for (;;) {
        BUILTIN_LED_TOGGLE();
        HAL_Delay(500);
    }
}

void Task_CheckButtonsEvents(void *arg)
{
    ButtonEvent_t event;

    for (;;) {
        for (Button_t btn = BTN0; btn <= BTN3; btn++) {
            event = Button_GetEvent(btn);
            if (event != ButtonEvent_NOTHING) {
                xTimerReset(timerShutdown, portMAX_DELAY);
                xQueueSend(queueSystemEvents, &((struct SystemEvent) {
                    .type = SysEvent_BUTTON,
                    .data.btn = btn,
                    .data.ev = event
                }), 0);
            }
        }
        OS_Wait(50);
    }
}

void Task_ApplicationEventLoop(void *arg)
{
    struct SystemEvent systemEvent;

    for (;;) {
        xQueueReceive(queueSystemEvents, &systemEvent, portMAX_DELAY);
        if (systemEvent.type == SysEvent_WAKEUP) {
            Button0_DisableInterrupt();
        }

        GFX_Clear();
        GFX_SetCursor(0, 0);
        GFX_PrintChar(Button_IsPressed(BTN0) ? '1' : '0');
        GFX_PrintChar(Button_IsPressed(BTN1) ? '1' : '0');
        GFX_PrintChar(Button_IsPressed(BTN2) ? '1' : '0');
        GFX_PrintChar(Button_IsPressed(BTN3) ? '1' : '0');
        GFX_PrintChar('0' + systemEvent.type);
        
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void Task_TerminalService(void *arg) // TODO
{
    extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
    static char buffer[APP_RX_DATA_SIZE];

    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        memcpy(buffer, UserRxBufferFS, sizeof(buffer));
        //CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer));
    }
}

void Callback_MeasureVoltage(TimerHandle_t timer)
{
    ADC_PerformMeasurements();
}

void Callback_CustomRoutine(TimerHandle_t timer)
{
    xQueueSend(queueSystemEvents, &((struct SystemEvent) {
        .type = SysEvent_CUSTOM,
    }), 0);
}

void Callback_Shutdown(TimerHandle_t timer)
{
    struct SystemEvent systemEvent = {.type = SysEvent_SUSPEND};

    if (Button_IsPressed(BTN0)) {
        systemEvent.type = SysEvent_SHUTDOWN;
        
        if (Button_IsPressed(BTN1)) {// Extra power cutoff
            systemEvent.type = SysEvent_ALARM;
            PWRLATCH_OFF();
        }
    }
    
    xQueueSend(queueSystemEvents, &systemEvent, portMAX_DELAY);
}

static bool PowerOnSelfTest(void)
{
    DISPLAY_Init();
    Button0_DisableInterrupt();

    if (!LL_GPIO_IsInputPinSet(IR_EYE_GPIO_Port, IR_EYE_Pin)) {
        ReportError("EYE");
        HAL_Delay(2000);
        // return false;
    }
    return true;
}

static void SystemLoad(void)
{
    // TODO load data from EEPROM
    GFX_Clear();
    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_PrintString("DxWatch");
    GFX_SetCursor(2, 1);
    GFX_PrintString("MK 1");
    
    GFX_SetCursor(0, 3);
    for (int i = 0; i < 5; i++) {
        GFX_PrintChar('.');
        HAL_Delay(200);
    }
    GFX_Clear();
}

void OS_Init(void)
{
    if (PowerOnSelfTest()) {
        PWRLATCH_ON();
    } else {
        Error_Handler();
    }
    SystemLoad();
    
    xTaskCreate(Task_CheckButtonsEvents,
            "cbe", 64, NULL, 0, &taskButtonsEvents);
    xTaskCreate(Task_ApplicationEventLoop,
            "app", 128, NULL, 1, &taskApplicationEventLoop);
    xTaskCreate(Task_TerminalService,
            "ter", 64, NULL, 2, &taskTerminalService);
    
    timerMeasureVoltage = xTimerCreate("vol", pdMS_TO_TICKS(10000), 
            true, NULL, Callback_MeasureVoltage);
    timerCustomRoutine = xTimerCreate("cus", 100, 
            false, NULL, Callback_CustomRoutine);
    timerShutdown = xTimerCreate("shd", pdMS_TO_TICKS(3000), 
            false, NULL, Callback_Shutdown);
    
    queueSystemEvents = xQueueCreate(SYSEVENT_QUEUE_LENGTH, sizeof(struct SystemEvent));

    xTimerStart(timerMeasureVoltage, 0);
    xQueueSend(queueSystemEvents, &((struct SystemEvent) {.type = SysEvent_STARTUP}), 0);
}

void OS_StartSheduler(void)
{
    vTaskStartScheduler();
}

void OS_Wait(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}
