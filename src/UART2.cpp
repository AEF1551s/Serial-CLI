#include "UART2.h"

/*
Used for configuration and initialization of USART2/UART2 peripheral

UART2 pin config should not be changable, because of used board.
Exception: if SolderBridge SB13 and SB14 are open, SB62 and SB63 are closed.
Then USART2 can be used with pin PA2, PA3 for alternative purpose.
*/

UART2::UART2()
{
    init();
}

inline void UART2::pinInit()
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

inline void UART2::init()
{
    // Enable RCC clock
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);

    pinInit();

    // TODO: SET AS BUILD OPTION
    //  Set baudrate to 115200b/s with oversampling 16 OVER8=0 at 16MHz
    WRITE_REG(USART2->BRR, 0x8B);

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
inline void UART2::interruptInit()
{
    SET_BIT(USART2->CR1, USART_CR1_RXNEIE);
    SET_BIT(USART2->CR1, USART_CR1_IDLEIE);
    
    //TODO: CHECK PRIORITY
   NVIC_EnableIRQ(USART2_IRQn);
}
void UART2::write(char ch)
{
    // Make sure the transmit data register is empty
    while (!(READ_BIT(USART2->SR, USART_SR_TXE)))
        ;

    // Write to transmit data register
    USART2->DR = static_cast<uint8_t>(ch);
}

char UART2::read()
{
    // Make sure the receive data register is not empty
    while (!READ_BIT(USART2->SR, USART_SR_RXNE))
        ;

    // Read receive data register
    return USART2->DR;
}
