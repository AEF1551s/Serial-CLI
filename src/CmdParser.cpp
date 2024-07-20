#include <CmdParser.h>

CmdParser::CmdParser(Serial &serial) : serial_(serial) {}

COMMAND CmdParser::scanCommand()
{
    //Reads command name with space.
    // Buffer for echo or set-led command
    char cmdBuffer[COMMAND_SIZE_MAX+1];
    
    //Scan until \r or full valid/invalid command
    bool checkCR = true;
    serial_.scan(cmdBuffer, COMMAND_SIZE_MAX+1, checkCR);

    static const char *cmd1 = "set-led ";
    static const char *cmd2 = "echo ";

    if (strncmp(cmd1, cmdBuffer, strlen(cmd1)) == 0)
        return SETLED;
    if (strncmp(cmd2, cmdBuffer, strlen(cmd2)) == 0)
        return ECHO;

    return NOCMD;
}

int CmdParser::getVariables(COMMAND cmd)
{
    switch (cmd)
    {
    case SETLED:
        /* code */
        break;
    case ECHO:
        /* code */
        break;
    default:
        return -1;
    }

    return 0;
}
