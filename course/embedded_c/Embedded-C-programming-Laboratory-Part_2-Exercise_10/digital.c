#include <msp430.h>
#include "digital.h"
#include <stdint.h>
#include <drawings.h>
#include <intrinsics.h> 

//L=P2.1, R=P1.1
// Pressed(0V)，Released(3.3V)
void Init_buttons(void) {
    P2DIR &= ~BIT1;  // P2.1 input button_L
    P2REN |= BIT1;
    P2OUT |= BIT1;
    
    P1DIR &= ~BIT1;  // P1.1 input button_R
    P1REN |= BIT1;
    P1OUT |= BIT1;
}

void Init_GPIO(void) {
    P2DIR |= BIT4 | BIT5;  // P2.4 (LED), P2.5 (Buzzer)
    P2OUT &= ~(BIT4 | BIT5); 
}

void Toggle_LED(void) {
    P2OUT ^= BIT4;
}

void Toggle_Buzzer(void) {
    if (state_button_L()) {
        P2OUT ^= BIT5;
    }
}

uint8_t state_button_L(void) {
    return (P2IN & BIT1) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t state_button_R(void) {
    return (P1IN & BIT1) ? 0 : 1; // Return 1 if the button is pressed
}



