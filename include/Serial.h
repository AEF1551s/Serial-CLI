#ifndef SERIAL_H
#define SERIAL_H

#include <customTypes.h>
#include <UART2.h>

#define INPUT_BUFFER_MAX 3000
#define OUTPUT_BUFFER_MAX 3000
#define COMMAND_SIZE_MAX 7 // set-led and echo

class Serial
{
    friend class CmdParser;

private:
    UART2 uart_;
    int scan(char *ptr, int len, bool cr);

public:
    int printString(const char *ptr);
    Serial(UART2 &uart2);
};

#endif // SERIAL_H
