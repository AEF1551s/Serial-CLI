#include "USART.h"

/*
Used for configuration and initialization of USART2/UART2 peripheral

UART2 pin config should not be changable, because of used board.
Exception: if SolderBridge SB13 and SB14 are open, SB62 and SB63 are closed.
Then USART2 can be used with pin PA2, PA3 for alternative purpose.
*/
USART::USART(int baudrate, USART_TypeDef *USART_REG) : baudrate_(baudrate), UART_REG(USART_REG)
{
    init();
}

void USART::pinInit()
{
    // Configure GPIOA 2, 3 pins for UART
    // Enable AHB1 clock for port A
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    // Set PA2 and PA3 to AF for UART2
    SET_BIT(GPIOA->MODER, 0b10 << GPIO_MODER_MODER2_Pos);
    SET_BIT(GPIOA->MODER, 0b10 << GPIO_MODER_MODER3_Pos);
    // Set AF7 (0b0111) to both pins (from Alternate Function Mapping)
    SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2); // AF7 = 0b0111
    SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFRL3_0 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_2); // AF7 = 0b0111
}

void USART::init()
{
    // Enable RCC clock
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);

    pinInit();

    //  Set baudrate to 115200b/s with oversampling 16 OVER8=0 at 16MHz
    // WRITE_REG(USART2->BRR, 0x8B);
    brToMantissa();
    WRITE_REG(USART2->BRR, mantissa);

    // Configure transmit enable
    SET_BIT(USART2->CR1, USART_CR1_TE);

    // Configure receive enable.
    SET_BIT(USART2->CR1, USART_CR1_RE);

    // Configure start, 1 stop and 8 data bits.
    CLEAR_BIT(USART2->CR1, USART_CR1_M);
    CLEAR_BIT(USART2->CR2, USART_CR2_STOP_0 | USART_CR2_STOP_1);

    // Enable RXNE interrupts
    interruptInit();

    // Enable UART;
    SET_BIT(USART2->CR1, USART_CR1_UE);
}
void USART::interruptInit()
{
    SET_BIT(USART2->CR1, USART_CR1_RXNEIE);
    SET_BIT(USART2->CR1, USART_CR1_IDLEIE);

    // Set priority to 92 under all used timers
    NVIC_SetPriority(USART2_IRQn, 92);
    NVIC_EnableIRQ(USART2_IRQn);
}
void USART::write(char ch)
{
    // Make sure the transmit data register is empty
    while (!(READ_BIT(USART2->SR, USART_SR_TXE)))
        ;

    // Write to transmit data register
    USART2->DR = static_cast<uint8_t>(ch);
}

char USART::read()
{
    // Make sure the receive data register is not empty
    while (!READ_BIT(USART2->SR, USART_SR_RXNE))
        ;

    // Read receive data register
    return USART2->DR;
}

void USART::brToMantissa()
{
    switch (baudrate_)
    {
    case 1200:
        // DIV 833.3125
        // 0d833 = 0x341
        // 16*0.3125 = 0d5 = 0x5
        mantissa = 0x3415;
        break;
    case 2400:
        // DIV 416.6875
        // 0d416 = 0x1A0
        // 16*0.6875 = 0d11 = 0xB
        mantissa = 0x1A0B;
        break;

    case 9600:
        // DIV 104.1875
        // 0d104 = 0x68
        // 16*0.1875 = 0d3 = 0x3
        mantissa = 0x683;
        break;

    case 19200:
        // DIV 52.0625
        // 0d52 = 0x34
        // 16*0.0625 = 0d1 = 0x1
        mantissa = 0x341;
        break;

    case 38400:
        // DIV 26.0625
        // 0d26 = 0x1A
        // 16*0.0625 = 0d1 = 0x1
        mantissa = 0x1A1;
        break;

    case 57600:
        // DIV 17.375
        // 0d17 = 0x11
        // 16*0.375 = 0d6 = 0x6
        mantissa = 0x116;
        break;

    case 115200:
        // DIV 8.6875
        // 0d8 = 0x8
        // 16*0.6875 = 0d11 = 0xB
        mantissa = 0x8B;
        break;

    case 230400:
        // DIV 4.3125
        // 0d4 = 0x4
        // 16*0.3125 = 0d5 = 0x5
        mantissa = 0x45;
        break;

    case 460800:
        // DIV 2.1875
        // 0d2 = 0x2
        // 16*0.1875 = 0d3 = 0x3
        mantissa = 0x23;
        break;

    case 921600:
        // DIV 1.0625
        // 0d1 = 0x1
        // 16*0.0625 = 0d1 = 0x1
        mantissa = 0x11;
        break;

    default:
        // Default set as 115200
        mantissa = 0x8B;
        break;
    }
}

USART_TypeDef *USART::getReg()
{
    return UART_REG;
}
