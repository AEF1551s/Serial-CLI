#ifndef SERIAL_CPP
#define SERIAL_CPP

// Header
#include <Serial.h>

char Serial::inputBuffer[INPUT_BUFFER_MAX];
int Serial::inputLen = 0;
int Serial::readBytes = 0;
bool Serial::overflow = false;
bool Serial::inputReady = false;
USART_TypeDef *Serial::usartReg = nullptr;

Serial::Serial(USART &serialUart) : uart_(serialUart)
{
    inputBuffer[INPUT_BUFFER_MAX - 1] = '\0';
    usartReg = uart_.getReg();
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
    return -1;
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
// Handles Serial UART interrupt
void Serial::handleInterrupt(USART_TypeDef *USART_REG = usartReg)
{
    // Data ready to be read for receive data register
    if (READ_BIT(USART_REG->SR, USART_SR_RXNE))
    {
        inputReady = false;
        inputBuffer[readBytes] = USART_REG->DR;
        readBytes++;

        // Overflow condition
        if (readBytes >= INPUT_BUFFER_MAX)
        {
            overflow = true;
            readBytes = 0;
            inputLen = 0;
            inputReady = false;
        }

        // If there was received data, no need to check if it is IDLE
        return;
    }
    if (READ_BIT(USART_REG->SR, USART_SR_IDLE))
    {
        // IDLE flag is cleared by reading SR and then DR.
        inputLen = readBytes;
        readBytes = 0;
        inputReady = true;
        int temp = USART_REG->DR;

        // When receiving is over and there was overflow, reset overflow in main,
        if (overflow == true)
        {
            inputLen = 0;
            readBytes = 0;
            inputReady = true;
        }
    }
}
void Serial::printError()
{
    printString(errorString);
}
void Serial::printOk()
{
    printString(okString);
}
// Get inputReady flag
bool Serial::getInputReady()
{
    return inputReady;
}
// Get overflow flas
bool Serial::getOverflow()
{
    return overflow;
}
// Set inputReady flag
void Serial::setInputReady(bool value)
{
    inputReady = value;
}
// Set overflow flag
void Serial::setOverflow(bool value)
{
    overflow = value;
}

//  Interrupts
extern "C" void USART2_IRQHandler(void)
{
    Serial::handleInterrupt(USART2);
}
#endif // SERIAL_CPP
