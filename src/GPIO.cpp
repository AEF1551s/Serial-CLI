#include <GPIO.h>

// Constructor with all data for pins
GPIO::GPIO(Timer &timer, LedPin led1, LedPin led2, LedPin led3, LedPin led4)
{
    this->ledArray[0] = led1;
    this->ledArray[1] = led2;
    this->ledArray[2] = led3;
    this->ledArray[3] = led4;
    this->timer_ = timer;
    init();
    // Set timer masks in timerInterrupts.h
    timer_.setBSRRMasks(led1.pin, led2.pin, led3.pin, led4.pin);
    timer_.setGPIOMasks(led1.GPIOx, led2.GPIOx, led3.GPIOx, led4.GPIOx);
}
void GPIO::init()
{
    initClock();
    initLedPins();
}
// Initializes clock for used GPIO ports
void GPIO::initClock()
{
    // All GPIOx uses AHB1
    int gpioEnableMask = 0;
    for (int i = 0; i < ledCount_; i++)
    {
        // For A, B, C ports
        if (ledArray[i].GPIOx == GPIOA)
        {
            gpioEnableMask = RCC_AHB1ENR_GPIOAEN;
            SET_BIT(RCC->AHB1ENR, gpioEnableMask);
        }
        else if (ledArray[i].GPIOx == GPIOB)
        {
            gpioEnableMask = RCC_AHB1ENR_GPIOBEN;
            SET_BIT(RCC->AHB1ENR, gpioEnableMask);
        }
        else if (ledArray[i].GPIOx == GPIOC)
        {
            gpioEnableMask = RCC_AHB1ENR_GPIOCEN;
            SET_BIT(RCC->AHB1ENR, gpioEnableMask);
        }
    }
}
// Initializes output mode for LED pins
void GPIO::initLedPins()
{
    unsigned int modePin = 0; // Pin offset. Each mode is 2 bits. Each mode start from 2*pin
    unsigned int mode = 0b01; // Output mode is 0b01

    for (int i = 0; i < ledCount_; i++)
    {
        modePin = ledArray[i].pin * 2;
        SET_BIT(ledArray[i].GPIOx->MODER, mode << modePin);
        modePin = 0;
    }
}
// Control of LEDs
void GPIO::ledControl(bool state, int ledId, int timeMs)
{
    if (state == true)
    {
        // Turn ON
        unsigned int setPin = ledArray[ledId - 1].pin;
        SET_BIT(ledArray[ledId - 1].GPIOx->BSRR, 1U << setPin);
        // Pass time to apropriate timer ARR, after that the timer turn off its assigned pin.
        WRITE_REG(timer_.LEDTimers[ledId - 1]->ARR, timeMs);
        // Reset counter, if to override previous led counter.
        CLEAR_REG(timer_.LEDTimers[ledId - 1]->CNT);
        // Enable counter
        SET_BIT(timer_.LEDTimers[ledId - 1]->CR1, TIM_CR1_CEN);
    }
}