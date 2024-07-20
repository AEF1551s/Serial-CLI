#ifndef UART2_H
#define UART2_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>

// USART2 used ONLY for serial communication with ST-LINK.

class UART2
{
public:
    UART2();
    void write(char ch);
    char read();

private:
    // Configure PA2 and PA3 to AF USART2 TX/RX
    inline void pinInit();
    // Message format 8 data bits, 1 stop bit; baudrate; full-duplex;
    inline void init();
};

#endif // UART2_H