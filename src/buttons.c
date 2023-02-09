#include <buttons.h>
#include <main.h>

#define DEBOUNCE_DELAY  5

static struct Button {
    GPIO_TypeDef *gpio;
    uint32_t pinMask;
    uint32_t lastCheckTime;
    bool lastState;
    bool activeLevel;
} buttons[4] = {
    {BTN_0_GPIO_Port, BTN_0_Pin, .activeLevel = 0},
    {BTN_1_GPIO_Port, BTN_1_Pin, .activeLevel = 1},
    {BTN_2_GPIO_Port, BTN_2_Pin, .activeLevel = 1},
    {BTN_3_GPIO_Port, BTN_3_Pin, .activeLevel = 1}
};

bool Button_IsPressed(Button_t btn)
{
    return LL_GPIO_IsInputPinSet(buttons[btn].gpio, buttons[btn].pinMask) == buttons[btn].activeLevel;
}

ButtonEvent_t Button_GetEvent(Button_t btn)
{
    ButtonEvent_t event = ButtonEvent_NOTHING;
    bool state = Button_IsPressed(btn);
    struct Button *button = &buttons[btn];

    if (state != button->lastState && HAL_GetTick() - button->lastCheckTime > 5) {
        button->lastCheckTime = HAL_GetTick();
        if (button->lastState == false && state == true)
            event = ButtonEvent_PRESS;
        else if (button->lastState == true && state == false)
            event = ButtonEvent_RELEASE;
        button->lastState = state;
    }
    
    return event;
}

void Button0_EnableInterrupt(void)
{
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_3);
}

void Button0_DisableInterrupt(void)
{
    LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_3);
}
