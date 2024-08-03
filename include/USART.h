#ifndef USART_H
#define USART_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>

// Message format 8 data bits, 1 stop bit; full-duplex; 2 pins on same PORT;

class USART
{
private:
    // Variables
    USART_TypeDef *USART_REG;
    int baudrate_;
    uint32_t mantissa;
    // Methods
    void init();
    void interruptInit();

public:
    USART(int baudrate, USART_TypeDef *USART_REG);
    void write(char ch);
    char read();
    void brToMantissa();
    USART_TypeDef *getReg();
};

#endif // USART_H