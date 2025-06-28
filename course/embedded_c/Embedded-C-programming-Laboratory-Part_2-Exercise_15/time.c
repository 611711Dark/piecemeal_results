#include <msp430.h>
#include "time.h"
#include "digital.h"

volatile uint8_t FLAG = 0;//global
static volatile uint8_t FLAG_T = 0;


// SMCLK = 8MHz, divided by 8 = 1MHz (1us/tick)
void Init_timer_no_interrupts(void) {
    TA1CTL = TASSEL_2 | ID_3 | MC_0 | TACLR; // SMCLK/8, stopped
}

void Init_pwm_red(void) {
    // PWM 800Hz, 25% duty
    P2SEL |= BIT4;                 // P2.4
    P2DIR |= BIT4;
    
    TA2CCR0 = 1250 - 1;                 // 1/(800Hz) = 1.25ms
    TA2CCR1 = 250 - 1;                 // 25% of 1249
    
    TA2CCTL1 = OUTMOD_7;   
    TA2CTL = TASSEL_2 | ID_3 | MC_1 | TACLR;
}

int8_t pwm_red(int8_t duty) {
    if (duty < 0 || duty > 100) return 1; // Error
    
    if (duty < 5) {
        TA2CCTL1 &= ~OUTMOD_7;     // Mode 0
        TA2CCTL1 &= ~OUT;          // LED off
    } 
    else if (duty > 95) {
        TA2CCTL1 &= ~OUTMOD_7;     // Mode 0
        TA2CCTL1 |= OUT;           // LED on
    } 
    else {
        TA2CCTL1 &= ~OUTMOD_7;   
        TA2CCTL1 |= OUTMOD_7;      // Mode 7（PWM）
        TA2CCR1 = duty * (TA2CCR0/100);
    }
    return 0;
}

void Timer_Delay_25ms(void) {
    TA1CCR0 = 24999;              // 25ms @ 1MHz: (25ms / 1us) - 1 = 24999
    TA1CCTL0 &= ~CCIFG;           // clear
    TA1CTL |= TACLR | MC_1; 
    
    while (!(TA1CCTL0 & CCIFG));  // wait flag
    TA1CTL &= ~MC_3;              // stop timer
}

void Run_timer_interrupt_25ms(void) {
    TA1CCR0 = 24999;
    TA1CCTL0 = CCIE;              // enable CCR0 interrupt
    TA1CTL = TASSEL_2 | ID_3 | MC_1 | TACLR;
}

uint8_t get_and_clear_FLAG_T(void) {
    uint16_t GIE_state = __get_SR_register() & GIE; 
    __disable_interrupt();
    
    uint8_t temp = FLAG_T;
    FLAG_T = 0;
    
    __bis_SR_register(GIE_state); // recover
    return temp;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void) {
    static unsigned int counter = 0;
    
    if (++counter >= 48) {        // 48 * 25ms = 1200ms
        FLAG_T = 1;
        FLAG = 1;  
        counter = 0;
    }
    Toggle_Buzzer();
    TA1CCTL0 &= ~CCIFG;
}
