#include <CmdParser.h>

CmdParser::CmdParser(Serial &serial, char *inputBuffer) : serial_(serial), inputBuffer_(inputBuffer) {}

COMMAND CmdParser::getCommand()
{
    static const char *cmd1 = "set-led ";
    static const char *cmd2 = "echo ";

    //Check command
    if (strncmp(cmd1, inputBuffer_, strlen(cmd1)) == 0)
        return SETLED;
    if (strncmp(cmd2, inputBuffer_, strlen(cmd2)) == 0)
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
