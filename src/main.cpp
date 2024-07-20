// CMSIS
#include <stm32f410rx.h>
#include <stm32f4xx.h>
// STD
// USER
#include <UART2.h>
#include <Serial.h>
#include <CmdParser.h>

// System clock by default is provided by HSI = 16MHz. Changing this will generate problems in all peripherals which use AHB, APBx bus.

//Maximum amount of input data
char* inputBuffer[3000];

// Main
int main()
{
    // Object declarations
    UART2 serialUart;
    Serial serial(serialUart);
    CmdParser cmdParser(serial);
    while (1)
    {

    }
    return 0;
}

//Interrupts
//UART2 interrupt
