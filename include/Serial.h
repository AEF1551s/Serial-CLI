#ifndef SERIAL_H
#define SERIAL_H
// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// USER
#include <customTypes.h>
#include <USART.h>

// DEFINES
#define INPUT_BUFFER_MAX 311 // 309 + \r + \0
#define OUTPUT_BUFFER_MAX 308
#define COMMAND_SIZE_MAX 7 // set-led and echo

class Serial
{
    friend class CmdParser;

private:
    // Objects
    USART uart_;
    // Variables
    static USART_TypeDef *usartReg;
    static char inputBuffer[INPUT_BUFFER_MAX];
    static int inputLen;
    static int readBytes;
    static bool overflow;
    static bool inputReady;

    const char *errorString = "ERROR\r\n";
    const char *okString = "OK\r\n";
    // Methods
    int scan(char *ptr, int len, bool cr = false);

public:
    // Methods
    Serial(USART &serialUart);
    int printString(const char *ptr);
    void printError();
    void printOk();
    static void handleInterrupt(USART_TypeDef *USART_REG); // Handles UART interrupt
    static bool getInputReady();
    static bool getOverflow();
    static void setInputReady(bool value);
    static void setOverflow(bool value);
};

#endif // SERIAL_H
