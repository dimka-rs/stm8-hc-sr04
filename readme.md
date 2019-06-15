Ultrasonic range meter on STM8+HC-SR04
=====

HC-SR04
-------
- Range: 2-400 cm
- Trigger: 10+ us
- Interval: 60+ ms
- Range [cm] = X us / 58
- Echo pulse length: 116-23200 us
- Delay until echo: ~480 us

STM8
====

Clock
-----
- HSI = 16 MHz
- HSI DIV = 8
- MASTER CLK = 2 MHz
- CPU DIV = 1
- CPU CLK = 2 MHz

UART
----
- PD5 - TX
- PD6 - RX

GPIO
----
- PB5 - LED
- PD2 - Trigger
- PD3 - Echo (TIM2_CH2)

TIM2
----
Measures echo delay
- TIM2 Prescaler = 2
- TIM2 CLK = 1 MHz
- TIM2 Tick = 1 us
- TIM2 Max Interval = 65535 us = 65.5 ms

TIM4
----
Triggers measurement every ~ 1s