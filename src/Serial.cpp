#ifndef SERIAL_CPP
#define SERIAL_CPP

// Header
#include <Serial.h>

Serial::Serial(UART2 &uart) : uart_(uart)
{
    //End buffer with \0 so can use cstring functions with whole buffer if needed
    // inputBuffer[INPUT_BUFFER_MAX-1] = '\0';
}

int Serial::printString(const char *ptr)
{
    int i = 0;
    for (i = 0; i < OUTPUT_BUFFER_MAX; i++)
    {
        //  Make sure the transmit data register is empty
        while (!(READ_BIT(USART2->SR, USART_SR_TXE)))
            ;

        // Write to transmit data register
        USART2->DR = (uint8_t)((*ptr++));

        // TODO: if '\0' is in string, it will take the \0 as end of string. Fix?
        if (*ptr == '\0')
        {
            return 0;
        }
    }
    return 0;
}
void Serial::handleInterrupt(char input)
{
    // If somone sends alot of data, system can stop because it always stays on interrupts.
    // Possible fix: disable interrupts if buffer is overflown, display error and wait.
    // if (readBytes >= INPUT_BUFFER_MAX)
    //     return;

    // // Write received data into buffer
    // inputBuffer[readBytes] = input;
    // readBytes++;
}

int Serial::scan(char *ptr, int len, bool cr)
{
    int bytesRead = 0;
    for (int i = 0; i < len; i++)
    {
        ptr[i] = uart_.read();
        bytesRead++;

        if (cr == true && ptr[i] == '\r')
        {
            break;
        }
    }
    return bytesRead;
}

#endif // SERIAL_CPP
