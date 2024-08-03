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
void GPIO::UART2pinInit()
{
    // Configure PA2 and PA3 to AF USART2 TX/RX
    // Configure GPIOA 2, 3 pins for UART
    // Enable AHB1 clock for port A
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    // Set PA2 and PA3 to AF for UART2
    SET_BIT(GPIOA->MODER, 0b10 << GPIO_MODER_MODER2_Pos);
    SET_BIT(GPIOA->MODER, 0b10 << GPIO_MODER_MODER3_Pos);
    // Set AF7 (0b0111) to both pins (from Alternate Function Mapping)
    SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2); // AF7 = 0b0111
    SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFRL3_0 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_2); // AF7 = 0b0111
}
// Based on provided port, turn on clock source
// GPIO ports use AHB1 clock
void enableAHB1Clock(GPIO_TypeDef *port)
{
    switch ((uint32_t)port)
    {
    case (uint32_t)GPIOA_BASE:
        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
        break;
    case (uint32_t)GPIOB_BASE:
        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
        break;
    case (uint32_t)GPIOC_BASE:
        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
        break;
    default:
        return;
    }
}
/*RX PORT PIN; TX PORT PIN; RTS PORT PIN; CTS PORT PIN; CK PORT PIN */
void GPIO::configUSARTxPins(GPIO_TypeDef *RXGPIO, int rxPin, int rxAf,
                            GPIO_TypeDef *TXGPIO, int txPin, int txAf,
                            GPIO_TypeDef *CKGPIO = nullptr, int ckPin = -1, int ckAf = -1,
                            GPIO_TypeDef *RTSGPIO = nullptr, int rtsPin = -1, int rtsAf = -1,
                            GPIO_TypeDef *CTSGPIO = nullptr, int ctsPin = -1, int ctsAf = -1)
{
    // Enable clock for ports
    // Set pins to AF
    // Set AF for pins
}
