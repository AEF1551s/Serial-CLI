#include <CmdParser.h>

CmdParser::CmdParser(Serial &serial, GPIO &gpio) : serial_(serial), gpio_(gpio) {}

COMMAND CmdParser::getCommand()
{
    static const char *cmd1 = "set-led ";
    static const char *cmd2 = "echo ";
    // Check command
    if (std::strncmp(cmd1, serial_.inputBuffer, strlen(cmd1)) == 0)
        return SETLED;
    if (std::strncmp(cmd2, serial_.inputBuffer, strlen(cmd2)) == 0)
        return ECHO;

    return NOCMD;
}
int CmdParser::checkSetLedCmd()
{
    // Not pretty but works to self made cases

    /*TODO: implement with strtok - must add \0 at end of cmd.
    for set-led only, divides, everything, then check those seperate strings by themselves
    If strtok returns NULL no delimeter has been found.*/

    char timeValueRaw[5];
    int timeValueLen = 0;
    int time = 0;
    bool crPresent = false;

    // Check if the input lenght is above max
    if (serial_.inputLen > 15)
    {
        return -1;
    }

    // 9th place for number 1-4
    if (serial_.inputBuffer[8] >= '1' && serial_.inputBuffer[8] <= '4')
    {
        // write led id into command data
        ledCommanData.ledId = myCtoui(serial_.inputBuffer[8]);
    }
    else
    {
        return -1;
    }

    // 10th place for ,
    if (serial_.inputBuffer[9] != ',')
    {
        return -1;
    }

    // 11-14 for 0-5000 +/r
    strncpy(timeValueRaw, serial_.inputBuffer + 10, 5);
    for (int i = 0; i < 5; i++)
    {
        if (timeValueRaw[i] == '\r')
        {
            crPresent = true;
            break;
        }
        timeValueLen = i + 1;
    }

    // Check for /r at the end of time
    if (crPresent == false)
    {
        return -1;
    }

    // Get time int value
    if (timeValueLen == 0 || timeValueLen > 4)
    {
        return -1;
    }
    else
    {
        for (int i = 0; i < timeValueLen; i++)
        {
            // Check if value is with number, convert time value
            if (timeValueRaw[i] < '0' || timeValueRaw[i] > '9')
            {
                return -1;
            }
            time = time * 10 + (timeValueRaw[i] - 48);
        }
    }
    // Time from [1,5000)
    if (!(time < 1 || time >= 5000))
    {
        // Write time into led command data
        ledCommanData.timeMs = time;
    }
    else
    {
        return -1;
    }

    // Passes checks, return 0
    return 0;
}
int CmdParser::checkEchoCmd()
{
    // echo is checked before, get size of data and check. Returns dataSize in cstring with \0
    // Check data size
    char *cmdSize = std::strtok(serial_.inputBuffer + 4, " ,");
    int size = myStoui(cmdSize);
    if (size < 0 || size > 300)
    {
        return -1;
    }
    // Check if <data> ends with /r
    int indexStartData = std::strlen(cmdSize) + std::strlen("data: ,") - 1;
    int indexEndData = indexStartData + size - 1;

    if (serial_.inputBuffer[indexEndData + 1] != '\r')
    {
        return -1;
    }

    // Create buffer for output string "data: <data>\r\n\0"
    int outputBufferSize = size + 9;
    int outputBufferCurrentIndex = 0;
    char tempOutputBuffer[outputBufferSize];
    tempOutputBuffer[outputBufferSize - 1] = '\0'; // \0 for printString
    static const char *cmdString = "data: ";

    // Add "data: " to start of output buffer
    for (int i = 0; cmdString[i] != '\0'; i++)
    {
        tempOutputBuffer[i] = cmdString[i];
        outputBufferCurrentIndex++;
    }

    // Copy valid <data> from input buffer
    for (int i = indexStartData; i < indexEndData + 1; i++)
    {
        tempOutputBuffer[outputBufferCurrentIndex] = serial_.inputBuffer[i];
        outputBufferCurrentIndex++;
    }
    static const char *end = "\r\n\0";
    for (size_t i = 0; i < 3; i++)
    {
        tempOutputBuffer[outputBufferCurrentIndex] = end[i];
        outputBufferCurrentIndex++;
    }
    outputBuffer = tempOutputBuffer;

    return 0;
}
int CmdParser::getVariables(COMMAND cmd)
{
    switch (cmd)
    {
    case SETLED:
        currentCmd = SETLED;
        return checkSetLedCmd();
    case ECHO:
        currentCmd = ECHO;
        return checkEchoCmd();
    default:
        currentCmd = NOCMD;
        return -1;
    }

    return 0;
}
void CmdParser::executeCmd()
{
    switch (currentCmd)
    {
    case SETLED:
        serial_.printOk();
        gpio_.ledControl(true, ledCommanData.ledId, ledCommanData.timeMs);
        break;
    case ECHO:
        serial_.printString(outputBuffer);
        serial_.printOk();
        break;
    default:
        serial_.printError();
        break;
    }
}
int CmdParser::readCommand()
{
    return getVariables(getCommand());
}
LedCommandData CmdParser::getLedCmdData()
{
    return ledCommanData;
}
COMMAND CmdParser::getCurrentCmd()
{
    return currentCmd;
}
