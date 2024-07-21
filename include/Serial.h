#ifndef SERIAL_H
#define SERIAL_H

#include <customTypes.h>
#include <UART2.h>

#define OUTPUT_BUFFER_MAX 308
#define COMMAND_SIZE_MAX 7 // set-led and echo

class Serial
{
    friend class CmdParser;

private:
    UART2 uart_;
    int scan(char *ptr, int len, bool cr);
    int readBytes = 0;
public:
    // char inputBuffer[INPUT_BUFFER_MAX];
    Serial(UART2 &uart2);
    int printString(const char *ptr);
    void handleInterrupt(char input);
};

#endif // SERIAL_H
