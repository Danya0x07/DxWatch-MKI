#include <apps.h>
#include <motor.h>
#include <flash_led.h>

static AppRetCode_t process(AppSignal_t signal, void *io)
{
    FLASH_LED_OFF();
    MOTOR_ON();
    *((struct Application **)io) = &appHome;
    return AppRetCode_EXIT;
}

struct Application appVibro = {
    .name = "Vibro",
    .load = AppDummyCallback,
    .process = process,
    .save = AppDummyCallback
};