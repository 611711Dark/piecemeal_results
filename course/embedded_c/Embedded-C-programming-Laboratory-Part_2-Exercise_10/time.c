#include <msp430.h>
#include "time.h"

// SMCLK = 8MHz, divided by 8 = 1MHz (1us/tick)
void Init_timer_no_interrupts(void) {
    TA1CTL = TASSEL_2 | ID_3 | MC_0 | TACLR; // SMCLK/8, stopped
}

void Timer_Delay_25ms(void) {
    TA1CCR0 = 24999;              // 25ms --- 1MHz: (25ms / 1us) - 1 = 24999
    TA1CCTL0 &= ~CCIFG;
    TA1CTL |= TACLR | MC_1; 
    
    while (!(TA1CCTL0 & CCIFG));  // wait flag
    TA1CTL &= ~MC_3;              // stop timer
}
