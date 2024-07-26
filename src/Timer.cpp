#include <Timer.h>

int timer5BsrrMask;
int timer6BsrrMask;
int timer9BsrrMask;
int timer11BsrrMask;

GPIO_TypeDef *timer5Gpio;
GPIO_TypeDef *timer6Gpio;
GPIO_TypeDef *timer9Gpio;
GPIO_TypeDef *timer11Gpio;

Timer::Timer()
{
    init();
}

void Timer::init()
{
    // Initialize 4 avaliable timers for each pin.
    // TIM5; TIM6; TIM9; TIM11
    LEDTimers[0] = TIM5;
    LEDTimers[1] = TIM6;
    LEDTimers[2] = TIM9;
    LEDTimers[3] = TIM11;
    initClock();
    interruptInit();
}
void Timer::initClock()
{
    // By default uses HSI as clock source = 16MHz
    // TIM9 and TIM11  uses APB2
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM9EN | RCC_APB2ENR_TIM11EN);
    // TIM5 and TIM6 uses APB1
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN | RCC_APB1ENR_TIM6EN);
    // ARR preload is disabled(default) to support "overriding" previous set led time
    //  Update interrupt only from counter overflow/underflow
    for (int i = 0; i < 4; i++)
    {
        SET_BIT(LEDTimers[i]->CR1, TIM_CR1_URS);
    }
    // Set prescaler to 16000
    unsigned int prescaler = 16000 - 1;
    for (int i = 0; i < 4; i++)
    {
        SET_BIT(LEDTimers[i]->PSC, prescaler);
    }
}
void Timer::interruptInit()
{
    // TIM interrupts must have more priority then USART to provide turning LEDs in time

    // Force interrupt. Even tough interrupts are not enabled, this forces PSC to be loaded into shadow register.
    // Problem and solution is described in http://www.efton.sk/STM32/gotcha/g40.html
    for (int i = 0; i < 4; i++)
    {
        SET_BIT(LEDTimers[i]->EGR, TIM_EGR_UG);
    }
    //  Enable interrupts
    for (int i = 0; i < 4; i++)
    {
        SET_BIT(LEDTimers[i]->DIER, TIM_DIER_UIE);
    }

    // USART interrupt priority is set to 92. Below all Tims
    NVIC_EnableIRQ(TIM5_IRQn);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
}
void Timer::setBSRRMasks(int pin1, int pin2, int pin3, int pin4)
{
    int maskArray[4];
    int pinArray[4] = {pin1, pin2, pin3, pin4};

    for (int i = 0; i < 4; i++)
    {
        // BSRR  RESET mask for LED1 - LED4 in order
        int mask = 1U<<(pinArray[i] + 16);
        maskArray[i] = mask;
    }

    timer5BsrrMask = maskArray[0];
    timer6BsrrMask = maskArray[1];
    timer9BsrrMask = maskArray[2];
    timer11BsrrMask = maskArray[3];
}
void Timer::setGPIOMasks(GPIO_TypeDef *gpio1, GPIO_TypeDef *gpio2, GPIO_TypeDef *gpio3, GPIO_TypeDef *gpio4)
{
    // Must be from 1 to 4 order!
    timer5Gpio = gpio1;
    timer6Gpio = gpio2;
    timer9Gpio = gpio3;
    timer11Gpio = gpio4;
}