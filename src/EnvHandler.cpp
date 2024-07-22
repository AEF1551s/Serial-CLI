#include <EnvHandler.h>

EnvHandler::EnvHandler(int argc, char **argv) : argc_(argc), argv_(argv)
{
}

int EnvHandler::stoui(char *s, char eos)
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

int EnvHandler::checkPortPin(char *s)
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
    int intPinId = stoui(pinId);

    // Each port has 15 pins
    if (intPinId < 0 || intPinId > 14)
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

int EnvHandler::parseArgs()
{
    // TODO: Change from PA1... to ledpin1 = PA1...
    //  Check if all arguments have been provided - name, 4 pins and baudrate
    if (argc_ != 6)
    {
        return -1;
    }

    char *s;
    // Check pin arguments
    for (int i = 1; i < 5; i++)
    {
        s = argv_[i];

        if (checkPortPin(s) == -1)
        {
            return -1;
        }
    }
    // Check baudrate
    s = argv_[5];

    return 0;
}

LedPin EnvHandler::getPin(int ledId)
{
    if (ledId < 1 || ledId > 4)
    {
        // Return first pin if incorrect ledId just in case;
        return ledPins[0];
    }

    return ledPins[ledId];
}
