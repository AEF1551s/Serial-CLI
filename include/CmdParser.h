#if !defined(CMD_PARSER_H)
#define CMD_PARSER_H

#include <cstring>

#include <customTypes.h>
#include <Serial.h>
#include <GPIO.h>
#include <myStoi.h>

class CmdParser
{
private:
    // Variables
    LedCommandData ledCommanData;
    COMMAND currentCmd;
    char *outputBuffer;
    // Objects
    Serial serial_;
    GPIO gpio_;
    // Methods
    COMMAND getCommand();
    int getVariables(COMMAND cmd);
    int checkSetLedCmd();
    int checkEchoCmd();

public:
    CmdParser(Serial &serial, GPIO &gpio);
    int readCommand();
    LedCommandData getLedCmdData();
    COMMAND getCurrentCmd();
    void executeCmd();
};

#endif // CMD_PARSER_H
