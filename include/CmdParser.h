#if !defined(CMD_PARSER_H)
#define CMD_PARSER_H

#include <cstring>

#include <customTypes.h>
#include <Serial.h>

class CmdParser
{
private:
    // Variables
    char *inputBuffer_;
    int &inputLen_;
    LedCommandData ledCommanData;
    // Objects
    Serial serial_;
    // Methods
    COMMAND getCommand();
    int getVariables(COMMAND cmd);
    int ctoui(char c); // char to unsigned int
    int checkSetLedCmd();
    int checkEchoCmd();
    int stoui(char *s, char eos = '\0');

public:
    CmdParser(Serial &serial, char *inputBuffer, int &inputLen);
    int readCommand();
    LedCommandData getLedCmdData();
};

#endif // CMD_PARSER_H
