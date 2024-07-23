#include <BuildOptionsParser.h>

BuildOptionsParser::BuildOptionsParser(/* args */)
{
}

int BuildOptionsParser::stoui(char *s, char eos)
{
    int result = 0;
    for (int i = 0; s[i] != eos; i++)
    {
        // Char check
        if (s[i] < '0' || s[i] > '9')
        {
            return -1;
        }
        result = result * 10 + (s[i] - 48);
    }
    return result;
}

int BuildOptionsParser::parsePortPin(char *s)
{
    static int pinCount = 0;
    if (pinCount >= 4)
    {
        return -1;
    }
    // Check lenght. Max lenght is for PX14 min is PX1

    int argLen = std::strlen(s);
    if (argLen < 3 || argLen > 4)
    {
        return -1;
    }

    if (s[0] != 'P')
    {
        return -1;
    }

    GPIO_TypeDef *gpiox;
    switch (s[1])
    {
    case 'A':
        gpiox = GPIOA;
        break;
    case 'B':
        gpiox = GPIOB;
        break;
    case 'C':
        gpiox = GPIOC;
        break;
    default:
        return -1;
    }

    char *pinId = s + 2;
    int intPinId = stoui(pinId); // Checks from PA to \0 which is pin number

    // Each port has 15 pins
    if (intPinId < 0 || intPinId > 15)
    {
        return -1;
    }

    // Pin and port are valid.
    ledPins[pinCount].ledId = pinCount;
    ledPins[pinCount].pin = intPinId;
    ledPins[pinCount].GPIOx = gpiox;

    pinCount++;
    return 0;
}
int BuildOptionsParser::parseBaudrate(int br)
{
    // Allows only these baudrates -
    // 1.2 KBps
    // 2.4 KBps
    // 9.6 KBps
    // 19.2 KBps
    // 38.4 KBps
    // 57.6 KBps
    // 115.2 KBps
    // 230.4 KBps
    // 460.8 KBps
    // 921.6 KBps

    switch (br)
    {
    case 1200:
        baudrate = 1200;
        return 0;

    case 2400:
        baudrate = 2400;
        return 0;

    case 9600:
        baudrate = 9600;
        return 0;

    case 19200:
        baudrate = 19200;
        return 0;

    case 38400:
        baudrate = 38400;
        return 0;

    case 57600:
        baudrate = 57600;
        return 0;

    case 115200:
        baudrate = 115200;
        return 0;

    case 230400:
        baudrate = 230400;
        return 0;

    case 460800:
        baudrate = 460800;
        return 0;

    case 921600:
        baudrate = 921600;
        return 0;

    default:
        return -1;
    }
}

LedPin BuildOptionsParser::getLedPin(int ledId)
{
    if (ledId < 1 || ledId > 4)
    {
        // Return first pin if incorrect ledId just in case;
        return ledPins[0];
    }
    return ledPins[ledId - 1];
}
