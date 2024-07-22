#if !defined(ENVHANDLER_H)
#define ENVHANDLER_H

// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// STD
#include <cstring>
// USER
#include <customTypes.h>

class EnvHandler
{
private:
    // Variables
    LedPin ledPins[4]; // 4 LED PINs and ports
    int baudrate;
    int argc_;
    char **argv_;
    // Methods
    int stoui(char *s, char eos = '\0');
    int checkPortPin(char *s);

public:
    EnvHandler(int argc, char **argv);
    int parseArgs();
    LedPin getPin(int ledId);
};

#endif // ENVHANDLER_H
