// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// STD
#include <cstring>
// USER
#include <UART2.h>
#include <Serial.h>
#include <CmdParser.h>

#define INPUT_BUFFER_MAX 308 // 307 + /0

// System clock by default is provided by HSI = 16MHz. Changing this will generate problems in all peripherals which use AHB, APBx bus.

// Global variables
volatile bool inputReady = false;
char inputBuffer[INPUT_BUFFER_MAX];

// Global object declarations
// UART2 serialUart;
// Serial serial(serialUart);

// Main
int main()
{

    // Object declarations

    UART2 serialUart;
    Serial serial(serialUart);
    CmdParser cmdParser(serial, inputBuffer);
    while (true)
    {
        inputReady = false;
        // Wait until rx serial port is idle
        while (!inputReady)
            ;

        COMMAND cmd = cmdParser.getCommand();

        switch (cmd)
        {
        case SETLED:
            serial.printString("SETLED INPUT\n");
            break;
        case ECHO:
            serial.printString("ECHO INPUT\n");
            break;
        default:
            serial.printString("ERROR\n");
            break;
        }
    }
    return 0;
}

// TODO: see how to implement with serial class
//  Interrupts
extern "C" void USART2_IRQHandler(void)
{
    volatile static int readBytes = 0;

    // Data ready to be read for receive data register
    if (READ_BIT(USART2->SR, USART_SR_RXNE))
    {
        inputReady = false;
        // serial.handleInterrupt(USART2->DR);
        inputBuffer[readBytes] = USART2->DR;
        readBytes++;
    }
    if (READ_BIT(USART2->SR, USART_SR_IDLE))
    {
        //IDLE flag is cleared by reading SR and then DR.
        readBytes = 0;
        inputReady = true;
        int temp = USART2->DR;
    }
}