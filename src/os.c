#include "os.h"
#include "buttons.h"
#include "adc.h"
#include "pwrlatch.h"
#include "display.h"
#include "builtin_led.h"
#include "eeprom.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <queue.h>

#include <gfx.h>
#include <apps.h>

TaskHandle_t taskButtonsEvents;
TaskHandle_t taskApplicationEventLoop;

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
    static const AppSignal_t BTN_SIGNALS[4][2] = {
        [BTN0] = {AppSignal_BTN0PRESS, AppSignal_BTN0RELEASE},
        [BTN1] = {AppSignal_BTN1PRESS, AppSignal_BTN1RELEASE},
        [BTN2] = {AppSignal_BTN2PRESS, AppSignal_BTN2RELEASE},
        [BTN3] = {AppSignal_BTN3PRESS, AppSignal_BTN3RELEASE}
    };
    
    struct SystemEvent systemEvent;
    struct Application *app = APP_LIST[0];
    AppRetCode_t retCode;
    int32_t output = 0;
    bool lowVoltage = false;

    for (;;) {
        xQueueReceive(queueSystemEvents, &systemEvent, portMAX_DELAY);
        output = 0;
        retCode = AppRetCode_OK;

        switch (systemEvent.type)
        {
        case SysEvent_STARTUP:
            for (int i = 0; i < APP_NUM; i++) {
                APP_LIST[i]->load();
            }
            app = APP_LIST[0];
            retCode = app->process(AppSignal_ENTRANCE, &output);
            xTimerStart(timerShutdown, portMAX_DELAY);
            break;
        
        case SysEvent_SHUTDOWN:
            for (int i = 0; i < APP_NUM; i++) {
                APP_LIST[i]->save();
            }
            xTimerStop(timerShutdown, portMAX_DELAY);
            xTimerStop(timerMeasureVoltage, portMAX_DELAY);
            xTimerStop(timerCustomRoutine, portMAX_DELAY);
            vTaskSuspend(taskButtonsEvents);
            Button0_DisableInterrupt();
            RTC_ALARM_Disable();
            GFX_Clear();
            PWRLATCH_OFF();
            break;
        
        case SysEvent_WAKEUP:
            Button0_DisableInterrupt();
            DISPLAY_ON();
            vTaskResume(taskButtonsEvents);
            xTimerReset(timerShutdown, portMAX_DELAY);
            retCode = app->process(AppSignal_WAKEUP, &output);
            break;

        case SysEvent_SUSPEND:
            retCode = app->process(AppSignal_SUSPEND, &output);
            if (retCode == AppRetCode_STAYUP)
                break;
            vTaskSuspend(taskButtonsEvents);
            xTimerStop(timerShutdown, portMAX_DELAY);
            DISPLAY_OFF();
            Button0_EnableInterrupt();
            break;

        case SysEvent_BUTTON:
        {
            AppSignal_t signal = BTN_SIGNALS[systemEvent.data.btn][systemEvent.data.ev - 1];
            retCode = app->process(signal, &output);
        }
            break;

        case SysEvent_CUSTOM:
            Button0_DisableInterrupt();
            DISPLAY_ON();
            vTaskResume(taskButtonsEvents);
            xTimerReset(timerShutdown, portMAX_DELAY);
            retCode = app->process(AppSignal_CUSTOM, &output);
            break;

        case SysEvent_VOLTAGE:
        {
            AppSignal_t signal = AppSignal_VOLTAGE;

            if (systemEvent.data.voltage <= 34 && !lowVoltage) {
                signal = AppSignal_LOWVOLTAGE;
                lowVoltage = true;
            } else if (systemEvent.data.voltage > 34) {
                lowVoltage = false;
            }
            retCode = app->process(signal, (void *)((intptr_t)systemEvent.data.voltage));
        }
            break;

        case SysEvent_ALARM:
            retCode = app->process(AppSignal_RTCALARM, &output);
            break;
        }

        while (retCode == AppRetCode_EXIT) {
            OS_StopCustomTimer();
            app = (struct Application *)output;
            retCode = app->process(AppSignal_ENTRANCE, &output);
        }
        if (retCode == AppRetCode_BAD) {
            // ?
        }
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
        
        if (Button_IsPressed(BTN1)) {  // Extra power cutoff
            PWRLATCH_OFF();
        }
    }
    
    xQueueSend(queueSystemEvents, &systemEvent, portMAX_DELAY);
}

static bool PowerOnSelfTest(void)
{
    DISPLAY_Init();
    EEPROM_Init();
    EEPROM_SetProtection(true);
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
    GFX_Clear();

    const char lastBuildDateTime[] = __DATE__ " " __TIME__;
    GFX_SetupBrush(GfxFontSize_7X5, GfxImageScale_X1, true);
    GFX_SetCursor(0, 5);
    GFX_PrintString(lastBuildDateTime);

    GFX_SetupBrush(GfxFontSize_14X10, GfxImageScale_X1, false);
    GFX_SetCursor(0, 0);
    GFX_PrintString("DxWatch");
    GFX_SetCursor(24, 2);
    GFX_PrintString("MK 1");
    
    GFX_SetCursor(0, 6);
    for (int i = 0; i < 5; i++) {
        GFX_PrintChar('.');
        HAL_Delay(400);
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
            "cbe", 64, NULL, 1, &taskButtonsEvents);
    xTaskCreate(Task_ApplicationEventLoop,
            "app", 256, NULL, 0, &taskApplicationEventLoop);
    
    timerMeasureVoltage = xTimerCreate("vol", pdMS_TO_TICKS(10000), 
            true, NULL, Callback_MeasureVoltage);
    timerCustomRoutine = xTimerCreate("cus", 100, 
            true, NULL, Callback_CustomRoutine);
    timerShutdown = xTimerCreate("shd", pdMS_TO_TICKS(5000), 
            true, NULL, Callback_Shutdown);
    
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

void OS_StartCustomTimer(uint32_t period)
{
    xTimerChangePeriod(timerCustomRoutine, pdMS_TO_TICKS(period), pdMS_TO_TICKS(20));
}

void OS_StopCustomTimer(void)
{
    xTimerStop(timerCustomRoutine, pdMS_TO_TICKS(20));
}