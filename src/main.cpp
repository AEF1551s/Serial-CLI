// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// STD
#include <cstring>
// USER
#include <UART2.h>
#include <Serial.h>
#include <CmdParser.h>
#include <GPIO.h>
// Build options and parser
#include <buildOptions.h>
#include <BuildOptionsParser.h>

#define INPUT_BUFFER_MAX 311 // 309 + \r + \0

// System clock by default is provided by HSI = 16MHz. Changing this will generate problems in all peripherals which use AHB, APBx bus.

// Response for 3000char overflow with -O0 is 260ms with 115200b/s.
// Theoretical end of Rx is 26ms.

// Global variables
volatile bool inputReady = false;
volatile bool overflow = false;

char inputBuffer[INPUT_BUFFER_MAX];
int inputLen = 0;
// Global object declarations
// UART2 serialUart;
// Serial serial(serialUart);

// Main
int main()
{
    // Object declarations
    BuildOptionsParser buildOptionsParser;

    // Check if all options are correct.
    int succ = 0;
    succ += buildOptionsParser.parsePortPin(LED1PIN);
    succ += buildOptionsParser.parsePortPin(LED2PIN);
    succ += buildOptionsParser.parsePortPin(LED3PIN);
    succ += buildOptionsParser.parsePortPin(LED4PIN);
    succ += buildOptionsParser.parseBaudrate(BAUDRATE);
    if (succ != 0)
        return -1;

    GPIO gpio(buildOptionsParser.getPin(1), buildOptionsParser.getPin(2), buildOptionsParser.getPin(3), buildOptionsParser.getPin(4));
    gpio.initLedPins();
    UART2 serialUart(BAUDRATE);
    Serial serial(serialUart);
    CmdParser cmdParser(serial, inputBuffer, inputLen);
    // Variables
    LedCommandData ledCmdData;
    const char *errorString = "ERROR\r\n";
    const char *okString = "OK\r\n";

    while (true)
    {
        inputReady = false;
        while (!inputReady)
            ;

        // Overflow response
        if (overflow)
        {
            serial.printString(errorString);
            overflow = false;
            continue;
        }
        // Command response
        if (cmdParser.readCommand() != NOCMD)
        {
            serial.printString(okString);
            // ledCmdData = cmdParser.getLedCmdData();
            // Enable led pin for time
        }
        else
        {
            serial.printString(errorString);
        }
    }
    return 0;
}

// TODO: see how to implement with serial class
//  Interrupts
extern "C" void USART2_IRQHandler(void)
{
    volatile static int readBytes = 0;
    inputLen = 0;
    // Data ready to be read for receive data register
    if (READ_BIT(USART2->SR, USART_SR_RXNE))
    {
        inputReady = false;
        // serial.handleInterrupt(USART2->DR);
        inputBuffer[readBytes] = USART2->DR;
        readBytes++;
        inputLen++;

        // Overflow
        if (readBytes >= INPUT_BUFFER_MAX)
        {
            // TODO: process input buffer overflow
            overflow = true;
            readBytes = 0;
            inputLen = 0;
            inputReady = false;
        }
    }
    if (READ_BIT(USART2->SR, USART_SR_IDLE))
    {
        // IDLE flag is cleared by reading SR and then DR.
        readBytes = 0;
        inputReady = true;
        int temp = USART2->DR;

        // When receiving is over and there was overflow, reset overflow in main,
        // input lenght and inputready here.
        if (overflow == true)
        {
            inputLen = 0;
            inputReady = true; // input is not valid to return error
        }
    }
}