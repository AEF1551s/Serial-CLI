#if !defined(CUSTOM_TYPES_H)
#define CUSTOM_TYPES_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>

// Input command types
typedef enum COMMAND
{
    NOCMD = -1,
    SETLED = 1,
    ECHO = 2

} COMMAND;

// Set-led command data to output
typedef struct LedCommandData
{
    int ledId;
    int timeMs;
} LedCommandData;

// GPIO port and PIN
typedef struct LedPin
{
    int ledId;           // [1,4]
    int pin;             // [0,15]
    GPIO_TypeDef *GPIOx; // [*GPIOA, *GPIOB, *GPIOC]
} LedPin;

#endif // CUSTOM_TYPES_H
