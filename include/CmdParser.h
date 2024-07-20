#if !defined(CMD_PARSER_H)
#define CMD_PARSER_H

#include <cstring>

#include <customTypes.h>
#include <Serial.h>

class CmdParser
{
private:
    Serial serial_;
    
public:
    COMMAND scanCommand();
    CmdParser(Serial &serial);
    int cmdSetLed(LedCommandData);
    int cmdEcho();
    int getVariables(COMMAND cmd);
};


#endif // CMD_PARSER_H
