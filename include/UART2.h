#ifndef UART2_H
#define UART2_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>

// USART2 used ONLY for serial communication with ST-LINK.

class UART2
{
    int baudrate_;
    uint32_t mantissa;
public:
    UART2(int baudrate, USART_TypeDef *USART_REG);
    void write(char ch);
    char read();
    void brToMantissa();
    USART_TypeDef* getReg();
private:
    USART_TypeDef* UART_REG;
    // Configure PA2 and PA3 to AF USART2 TX/RX
    inline void pinInit();
    // Message format 8 data bits, 1 stop bit; baudrate; full-duplex;
    inline void init();
    inline void interruptInit();
};

#endif // UART2_H