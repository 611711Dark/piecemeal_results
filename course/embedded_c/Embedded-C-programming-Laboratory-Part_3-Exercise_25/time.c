#include <msp430.h>
#include "time.h"
#include "digital.h"

static volatile uint8_t flag_40ms = 0;

// SMCLK = 8MHz, divided by 8 = 1MHz (1us/tick)
 
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


void Run_timer_interrupt_40ms(void) {
    TA1CCR0 = 39999;              // 40ms @ 1MHz: (40ms / 1us) - 1 = 40000-1
    TA1CCTL0 = CCIE;              // enable CCR0 interrupt
    TA1CTL = TASSEL_2 | ID_3 | MC_1 | TACLR;
}

uint8_t get_and_clear_flag_40ms(void) {
    uint16_t GIE_state = __get_SR_register() & GIE; 
    __disable_interrupt();
    
    uint8_t temp = flag_40ms;
    flag_40ms = 0;
    
    __bis_SR_register(GIE_state); // recover
    return temp;
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void) {
    flag_40ms = 1;
}

