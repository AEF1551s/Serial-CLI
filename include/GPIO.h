#if !defined(GPIO_H)
#define GPIO_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// USER
#include <customTypes.h>

class GPIO
{
private:
    /* data */
    LedPin led1_;
    LedPin led2_;
    LedPin led3_;
    LedPin led4_;

public:
    GPIO(LedPin led1, LedPin led2, LedPin led3, LedPin led4);
    void initLedPins();
};

#endif // GPIO_H
