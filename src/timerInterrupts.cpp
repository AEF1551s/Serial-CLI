#include <timerInterrupts.h>

extern "C"
{
    void printString(const char *ptr)
    {
        int i = 0;
        for (i = 0; i < 30; i++)
        {
            //  Make sure the transmit data register is empty
            while (!(READ_BIT(USART2->SR, USART_SR_TXE)))
                ;

            // Write to transmit data register
            USART2->DR = (uint8_t)((*ptr++));

            if (*ptr == '\0')
            {
                return;
            }
        }
    }

    // Timer intterrupts
    void TIM5_IRQHandler(void)
    {
        // LED1 timer interrupt
        if (TIM5->SR, TIM_SR_UIF)
        {
            SET_BIT(timer5Gpio->BSRR, timer5BsrrMask);
            // SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR_4);
            // Disable counter
            CLEAR_BIT(TIM5->CR1, TIM_CR1_CEN);
            // Clear Interrupt Flag
            CLEAR_BIT(TIM5->SR, TIM_SR_UIF);
            // Send led-off: 1
            printString("led-off: 1\r\n");
        }
    }
    void TIM6_DAC_IRQHandler(void)
    {
        // LED2 timer interrupt
        if (TIM6->SR, TIM_SR_UIF)
        {
            SET_BIT(timer6Gpio->BSRR, timer6BsrrMask);
            CLEAR_BIT(TIM6->CR1, TIM_CR1_CEN);
            CLEAR_BIT(TIM6->SR, TIM_SR_UIF);
            // Send led-off: 2
            printString("led-off: 2\r\n");

        }
    }
    void TIM1_BRK_TIM9_IRQHandler(void)
    {
        // LED3 timer interrupt
        if (TIM9->SR, TIM_SR_UIF)
        {
            SET_BIT(timer9Gpio->BSRR, timer9BsrrMask);
            CLEAR_BIT(TIM9->CR1, TIM_CR1_CEN);
            CLEAR_BIT(TIM9->SR, TIM_SR_UIF);
            // Send led-off: 3
            printString("led-off: 3\r\n");

        }
    }
    void TIM1_TRG_COM_TIM11_IRQHandler(void)
    {
        // LED4 timer interrupt
        if (TIM11->SR, TIM_SR_UIF)
        {
            SET_BIT(timer11Gpio->BSRR, timer11BsrrMask);
            CLEAR_BIT(TIM11->CR1, TIM_CR1_CEN);
            CLEAR_BIT(TIM11->SR, TIM_SR_UIF);
            // Send led-off: 4
            printString("led-off: 4\r\n");

        }
    }
}