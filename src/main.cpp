// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// STD
#include <cstring>
// USER
#include <USART.h>
#include <Serial.h>
#include <CmdParser.h>
#include <GPIO.h>
#include <customTypes.h>
#include <Timer.h>
#include <timerInterrupts.h>
// Build options and parser
#include <buildOptions.h>
#include <BuildOptionsParser.h>

// #define INPUT_BUFFER_MAX 311 // 309 + \r + \0

// System clock by default is provided by HSI = 16MHz. Changing this will generate problems in all peripherals which use AHB, APBx bus.

// Response for 3000char overflow with -O0 is 260ms with 115200b/s.
// Theoretical end of Rx is 26ms.

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

    Timer ledTimer;
    GPIO gpio(ledTimer, buildOptionsParser.getLedPin(1), buildOptionsParser.getLedPin(2), buildOptionsParser.getLedPin(3), buildOptionsParser.getLedPin(4));
    USART serialUart(BAUDRATE, USART2);
    Serial serial(serialUart);
    CmdParser cmdParser(serial);

    // Variables
    LedCommandData ledCmdData;
    const char *errorString = "ERROR\r\n";
    const char *okString = "OK\r\n";

    while (true)
    {

        // Get input ready and overflow from serial class
        Serial::setInputReady(false);
        while (!Serial::getInputReady())
            ;

        // Overflow response
        if (Serial::getOverflow())
        {
            serial.printString(errorString);
            Serial::setOverflow(false);
            continue;
        }

        // Command response
        if (cmdParser.readCommand() != -1)
        {
            serial.printString(okString);
            // Enable led pin for time
            if (cmdParser.getCurrentCmd() == SETLED)
            {
                LedCommandData cmdData;
                cmdData = cmdParser.getLedCmdData();
                gpio.ledControl(true, cmdData.ledId, cmdData.timeMs);
            }
        }
        else
        {
            serial.printString(errorString);
        }
    }
    return 0;
}