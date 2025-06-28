#include <msp430.h>
#include "digital.h"
#include <stdint.h>
#include <drawings.h>
#include <intrinsics.h> 

//S1=P4.0, S2=P3.7
// Pressed(0V)，Released(3.3V)
void Init_buttons(void) {
    P4DIR &= ~BIT0;  // P4.0 input
    P4REN |= BIT0;
    P4OUT |= BIT0;
    
    P3DIR &= ~BIT7;  // P3.7 input
    P3REN |= BIT7;
    P3OUT |= BIT7;
}

uint8_t state_button_down(void) {
    return (P3IN & BIT7) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t state_button_up(void) {
    return (P4IN & BIT0) ? 0 : 1; // Return 1 if the button is pressed
}

void Init_GPIO(void) {
    P2DIR |= BIT4 | BIT5;  // P2.4 (LED), P2.5 (Buzzer)
    P2OUT &= ~(BIT4 | BIT5); 
}

void Toggle_LED(void) {
    P2OUT ^= BIT4;
}

void Toggle_Buzzer(void) {
    if (state_button_up()) {
        P2OUT ^= BIT5;
    }
}


