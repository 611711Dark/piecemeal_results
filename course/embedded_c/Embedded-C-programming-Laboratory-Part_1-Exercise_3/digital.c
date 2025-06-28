#include <msp430.h>
#include "digital.h"
#include <stdint.h>
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

uint8_t state_button_L(void) {
    return (P3IN & BIT7) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t state_button_R(void) {
    return (P4IN & BIT0) ? 0 : 1; // Return 1 if the button is pressed
}
