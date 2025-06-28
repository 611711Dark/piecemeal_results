#include <msp430.h>
#include "time.h"
#include "digital.h"

volatile uint8_t FLAG = 0;//global
static volatile uint8_t FLAG_T = 0;
static volatile uint16_t new_duty = 0;

// SMCLK = 8MHz, divided by 8 = 1MHz (1us/tick)
void Init_timer_no_interrupts(void) {
    TA1CTL = TASSEL_2 | ID_3 | MC_0 | TACLR; // SMCLK/8, stopped
}
 
void Init_pwm_rgb(void) {
    // PWM 800Hz, 25% duty
    //P2.4-TA2.1-red, P1.5-TA0.4-green,  P1.4-TA0.3-blue
    P2DIR |= BIT4;
    P1DIR |= BIT4 | BIT5;

    P2SEL |= BIT4;
    P1SEL |= BIT4 | BIT5;

    TA2CCR0 = 1250 - 1;                 // 1/(800Hz) = 1.25ms
    TA2CCR1 = 312 - 1;                  // 25% of 1249  red
    TA2CCTL1 = OUTMOD_7;
    TA2CTL = TASSEL_2 | ID_3 | MC_1 | TACLR; 

    TA0CCR0 = 1250 - 1;                 // 1/(800Hz) = 1.25ms
    TA0CCR3 = 312 - 1;                 // 25% of 1249
    TA0CCR4 = 312 - 1; 
    TA0CCTL3 = OUTMOD_7;
    TA0CCTL4 = OUTMOD_7;
    TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR; 
}

int8_t pwm_led(LED_Channel channel, int8_t duty) {
    volatile uint16_t *CCR;
    volatile uint16_t *CCTL;
    
    switch(channel) {
        case LED_RED:
            CCR = &TA2CCR1;
            CCTL = &TA2CCTL1;
            break;
        case LED_BLUE:
            CCR = &TA0CCR3;
            CCTL = &TA0CCTL3;
            break;
        case LED_GREEN:
            CCR = &TA0CCR4;
            CCTL = &TA0CCTL4;
            break;
        default:
            return 1;
    }
    
    if (duty < 0 || duty > 100) return 1;
    
    if (duty < 5) {
        *CCTL &= ~OUTMOD_7;  
        *CCTL &= ~OUT;       
    } 
    else if (duty > 95) {
        *CCTL &= ~OUTMOD_7;  
        *CCTL |= OUT;        
    } 
    else {
        *CCTL &= ~OUTMOD_7;  
        *CCTL |= OUTMOD_7;   
        
        uint16_t ccr0 = (channel == LED_RED) ? TA2CCR0 : TA0CCR0;
        *CCR = duty * (ccr0 / 100);
    }
    
    return 0;
}


int8_t pwm_red_sync(int8_t duty) {
    if ( (duty < 0) || (duty > 100) ) return 1; // Error
    
    if (duty < 5) {
        TA2CCTL1 &= ~OUTMOD_7;     // Mode 0
        TA2CCTL1 &= ~OUT;          // LED off
    } 
    else if (duty > 95) {
        TA2CCTL1 &= ~OUTMOD_7;     // Mode 0
        TA2CCTL1 |= OUT;           // LED on
    } 
    else {
        new_duty = (uint16_t)(duty * (TA2CCR0 / 100));  
        TA2CCTL1 |= CCIE;                // Enable CCR1 interrupt
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

#pragma vector = TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void) {
    switch(TA2IV) {
        case TA2IV_TACCR1:          // CCR1 interrupt
            TA2CCTL1 &= ~OUTMOD_7;   
            TA2CCTL1 |= OUTMOD_7;      // Mode 7（PWM）
            TA2CCR1 = new_duty;     // Update safely
            TA2CCTL1 &= ~CCIE;      // Disable interrupt
            break;
    }
}
