#if !defined(TIMER_H)
#define TIMER_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// USER
#include <customTypes.h>
#include <timerInterrupts.h>



class Timer
{
    friend class GPIO;

private:
    // Variables
    TIM_TypeDef *LEDTimers[4];
    // Methods
    void init();
    void interruptInit();
    void initClock();
    void setBSRRMasks(int pin1, int pin2, int pin3, int pin4);
    void setGPIOMasks(GPIO_TypeDef *gpio1, GPIO_TypeDef *gpio2, GPIO_TypeDef *gpio3, GPIO_TypeDef *gpio4);

public:
    Timer();
    void setLedTimer(int ledId);
};

#endif // TIMER_H