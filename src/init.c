#include "stm8s.h"
#include "main.h"

void init_gpio()
{
    /* LED */
    LED_PORT->ODR |= (uint8_t)LED_PIN;      // default high
    LED_PORT->CR1 |= (uint8_t)LED_PIN;      // push-pull
    LED_PORT->CR2 &= (uint8_t)(~LED_PIN);   // slow
    LED_PORT->DDR |= (uint8_t)LED_PIN;      // output

    /* TRIG */
    TRIG_PORT->ODR &= (uint8_t)(~TRIG_PIN); // default low
    TRIG_PORT->CR1 |= (uint8_t)TRIG_PIN;    // push-pull
    TRIG_PORT->CR2 &= (uint8_t)(~TRIG_PIN); // slow
    TRIG_PORT->DDR |= (uint8_t)TRIG_PIN;    // output
}

void init_uart()
{
    /* UART 1 */
    // TX: PD5
    // RX: PD6

    // UART_DIV = f MASTER / baud rate
    // UART_DIV = 2 * 10^6 / 115200
    // UART_DIV = 17.36 = 0x0011 = 0x0 (BRR2_H), 0x01 (BRR1), 0x1 (BRR2_L)
    // prescaler updated after BRR1 write, so BRR2 is written first
    UART1->BRR2 = 0x01;
    UART1->BRR1 = 0x01;

    UART1->CR1 = 0; // enable, 8bit, no parity
    UART1->CR2 = UART1_CR2_TEN|UART1_CR2_REN; // rx+tx
    UART1->CR3 = 0; // 1 sb, no clk
    UART1->CR4 = 0; // no lin, no addr
    UART1->CR5 = 0; // no smart card, no half duplex, no irda
}

void init_tim4()
{
    // Timer 4
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_CEN); // disable

    // 2 MHz / 128 = 15625 Hz timer clock
    // count to 128 to get ~ 122 Hz resolution

    TIM4->PSCR = (uint8_t)(TIM4_PRESCALER_128);
    TIM4->ARR  = (uint8_t)(128);
    TIM4->SR1 &= (uint8_t)(~TIM4_FLAG_UPDATE); // clear it flag
    TIM4->IER |= (uint8_t)TIM4_IT_UPDATE; //enable it

    TIM4->CR1 |= (uint8_t)TIM4_CR1_CEN; //enable
}

void init_tim2()
{
    // Timer 2
    TIM2->PSCR = 1; // 2^1
    
    // Select active input
    TIM2->CCMR2 |= (TIM2_CCMR_CCxS & 1); // input, TI2FP2
    
    // Program filter
    // none by default
    // TIM2->CCMR2 bits IC2F

    // Select edge
    // rising by default
    // TIM2->CCER1 bit CC2P

    // Program prescaler
    // none by default
    // TIM2->CCMR2 bits IC2PSC

    // Enable capture
    TIM2->CCER1 |= TIM2_CCER1_CC2E;

    // Enable CC2 interrupt
    TIM2->IER |= TIM2_IER_CC2IE;

    TIM2->CR1 |= TIM2_CR1_CEN;
}