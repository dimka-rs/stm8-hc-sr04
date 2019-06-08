HC-SR04
-------

Range: 2-400 cm
Trigger: 10+ us
Interval: 60+ ms
Range [cm] = X us / 58
Echo pulse length: 116-23200 us
Delay until echo: ~480 us

HSI = 16 MHz
HSI DIV = 8
MASTER CLK = 2 MHz
CPU DIV = 1
CPU CLK = 2 MHz

TIM2 will measure echo delay

TIM2 Prescaler = 2
TIM2 CLK = 1 MHz
TIM2 Tick = 1 us
TIM2 Max Interval = 65535 us = 65.5 ms

TIM4 will trigger measurement