/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "init.h"
#include "stdio.h"

/* Private defines -----------------------------------------------------------*/
volatile uint8_t tim4flag = 1;
volatile uint16_t start = 0;
volatile uint16_t stop = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    init_uart();
    init_tim4();
    init_gpio();
    init_tim2();

    uint8_t * uid = (uint8_t *) 0x4865;
    uint8_t uid_size = 12;
    printf("\r\nUID: ");
    for (int i = 0; i < uid_size; uid++, i++) printf("%02X",*uid);
    printf("\r\n");

    enableInterrupts();
    while (1)
    {
        while (tim4flag);
        tim4flag = 1;
        LED_PORT->ODR &= ~(LED_PIN); // LED on
        TRIG_PORT->ODR |= TRIG_PIN; // TRIG high
        // ~10 us delay: 20 ops @ 2 MHz, 4 cycles * 5 ops
        for(volatile int8_t i = 0; i < 5; i++);
        //__asm__("ldw X, 20\n nop\n 00001$:\n decw X\n jrne 00001$\n nop\n ");
        TRIG_PORT->ODR &= ~(TRIG_PIN); // TRIG low
        TIM2->CR1 |= TIM2_CR1_CEN; // start timer 2
    }
}


/***********************************************************************/

void tim4_isr(void) __interrupt(ITC_IRQ_TIM4_OVF) {
    static uint8_t tim4cnt = 0;

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
    // get value on rising edge
    if(TIM2->SR1 & TIM2_SR1_CC1IF)
    {
        start = (TIM2->CCR1H) * 256;
        start += TIM2->CCR1L;
        //TIM2->EGR |= TIM2_EGR_UG; // reset counter
    }

    // read counter on falling edge
    if(TIM2->SR1 & TIM2_SR1_CC2IF)
    {
        stop = (TIM2->CCR2H) * 256;
        stop += TIM2->CCR2L;
        printf("%u\r\n", (stop-start)/58);
        LED_PORT->ODR |= LED_PIN; // LED off
        TIM2->CR1 &= ~TIM2_CR1_CEN; // stop timer 2
    }

    // clear all flags
    TIM2->SR1 = 0;
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
