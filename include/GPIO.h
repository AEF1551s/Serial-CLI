#if !defined(GPIO_H)
#define GPIO_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// USER
#include <customTypes.h>
#include <Timer.h>
// #include <timerInterrupts.h>

#define LEDCOUNT 4

class GPIO
{
private:
    // Variables
    LedPin ledArray[LEDCOUNT];
    const int ledCount_ = LEDCOUNT;
    // Methods
    void initLedPins();
    void initClock();
    void init();
    // Objects
    Timer timer_; //Used to initialize timerBSRR masks that are used in timer ISR

public:
    GPIO(Timer &timer, LedPin led1, LedPin led2, LedPin led3, LedPin led4);
    void ledControl(bool state, int ledId, int timeMs);
};

#endif // GPIO_H
