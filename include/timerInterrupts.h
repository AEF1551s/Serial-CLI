#if !defined(TIMER_INTERRUPTS_H)
#define TIMER_INTERRUPTS_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>

extern "C"
{
    // LED pin BSRR reset masks
    extern int timer5BsrrMask;
    extern int timer6BsrrMask;
    extern int timer9BsrrMask;
    extern int timer11BsrrMask;

    extern GPIO_TypeDef *timer5Gpio;
    extern GPIO_TypeDef *timer6Gpio;
    extern GPIO_TypeDef *timer9Gpio;
    extern GPIO_TypeDef *timer11Gpio;

    void printString(const char *ptr);
}

#endif // TIMER_INTERRUPTS_H
