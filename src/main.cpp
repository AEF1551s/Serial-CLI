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
#include <BuildOptionsParser.h>

// System clock by default is provided by HSI = 16MHz. Changing this will generate problems in all peripherals which use AHB, APBx bus.

// Response for 3000char overflow with -O0 is 260ms with 115200b/s.
// Theoretical end of Rx is 26ms.

// Main
int main()
{
    // Object declarations
    BuildOptionsParser buildOptionsParser;

    if (buildOptionsParser.checkBuildOptions() == -1)
    {
        return -1;
    }

    Timer ledTimer;
    GPIO gpio(ledTimer, buildOptionsParser.getLedPin(1), buildOptionsParser.getLedPin(2), buildOptionsParser.getLedPin(3), buildOptionsParser.getLedPin(4));
    USART serialUart(BAUDRATE, USART2);
    Serial serial(serialUart);
    CmdParser cmdParser(serial, gpio);

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

        // Overflow response w/ if statement
        if (Serial::getOverflow())
        {
            serial.printString(errorString);
            Serial::setOverflow(false);
            continue;
        }

        // // Overflow response w/ short-circuit evaluation
        // while (Serial::getOverflow() && (serial.printString(errorString), Serial::setOverflow(false), true))
        // {
        //     continue;
        // }

        // Command response
        if (cmdParser.readCommand() != -1)
        {
            cmdParser.executeCmd();
        }
        else
        {
            serial.printString(errorString);
        }
    }
    return 0;
}