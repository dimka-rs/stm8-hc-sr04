/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "init.h"
#include "stdio.h"

/* Private defines -----------------------------------------------------------*/
volatile uint8_t tim4flag = 1;
volatile uint8_t echoh = 0;
volatile uint8_t echol = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    init_uart();
    init_tim4();
    init_gpio();
    init_tim2();

    enableInterrupts();

    printf("Hello\r\n");
    while (1)
    {
        while (tim4flag);
        tim4flag = 1;
        printf("TIM4\r\n");

        LED_PORT->ODR &= ~(LED_PIN); // LED on
        TRIG_PORT->ODR |= TRIG_PIN; // TRIG high

        // ~10 us delay: 20 ops @ 2 MHz, 4 cycles * 5 ops
        for(volatile int8_t i = 0; i < 4; i++);
        //__asm__("ldw X, 20\n nop\n 00001$:\n decw X\n jrne 00001$\n nop\n ");

        TRIG_PORT->ODR &= ~(TRIG_PIN); // TRIG low
        LED_PORT->ODR |= LED_PIN; // LED off
    }
}


/***********************************************************************/

void tim4_isr(void) __interrupt(ITC_IRQ_TIM4_OVF) {
    static uint8_t tim4cnt = 0;

    //printf("!");
    tim4cnt++;
    // timer overflows at 122 Hz
    if(tim4cnt >= 122){
        tim4cnt = 0;
        tim4flag = 0;
    }
    // clear IT flag
    TIM4->SR1 &= (uint8_t)(~TIM4_IT_UPDATE);
}

void tim2_isr(void) __interrupt(ITC_IRQ_TIM2_CAPCOM) {
    // Read CCR
    echoh = TIM2->CCR2H;
    echol = TIM2->CCR2L;
    printf("%u %u\r\n", echoh, echol);
    // reset counter
    //TIM2->CR1 &= ~TIM2_CR1_CEN;
    //TIM2->CNTRH = 0;
    //TIM2->CNTRL = 0;
    TIM2->EGR |= TIM2_EGR_UG; //update values
    //TIM2->CR1 |= TIM2_CR1_CEN;
    // Clear CC2I flag
    TIM2->SR1 &= (~TIM2_SR1_CC2IF);
}

/* Debug output */
int putchar (int c)
{
    // this implementation is SDCC 3.8 specific
    // see sdcc stdio.h for details
    UART1->DR = (uint8_t) c;
    while (!(UART1->SR & UART1_FLAG_TXE));
    return c;
}


// This is called by some of the SPL files on error.
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  (void)file;
  (void)line;

  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
