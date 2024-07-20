#if !defined(CUSTOM_TYPES_H)
#define CUSTOM_TYPES_H

// Input command types
typedef enum COMMAND
{
    NOCMD = -1,
    SETLED = 1,
    ECHO = 2

} COMMAND;

// Set-led command data to output
struct LedCommandData
{
    int ledId;
    int timeMs;
};

#endif // CUSTOM_TYPES_H
