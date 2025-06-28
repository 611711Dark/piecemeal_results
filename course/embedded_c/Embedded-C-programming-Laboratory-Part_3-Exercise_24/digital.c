#include <msp430.h>
#include "digital.h"
#include <stdint.h>
#include <drawings.h>
#include <intrinsics.h> 

//S1=P4.0, S2=P3.7
//L=P2.1, R=P1.1
// Pressed(0V)，Released(3.3V)
void Init_buttons(void) {
    P4DIR &= ~BIT0;  // P4.0 input
    P4REN |= BIT0;
    P4OUT |= BIT0;
    
    P3DIR &= ~BIT7;  // P3.7 input
    P3REN |= BIT7;
    P3OUT |= BIT7;

    P2DIR &= ~BIT1;  // P2.1 input button_L
    P2REN |= BIT1;
    P2OUT |= BIT1;
    
    P1DIR &= ~BIT1;  // P1.1 input button_R
    P1REN |= BIT1;
    P1OUT |= BIT1;
}

uint8_t state_button_down(void) {
    return (P3IN & BIT7) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t state_button_up(void) {
    return (P4IN & BIT0) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t state_button_left(void) {
    return (P2IN & BIT1) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t state_button_right(void) {
    return (P1IN & BIT1) ? 0 : 1; // Return 1 if the button is pressed
}

void Init_rgb_led(void) {
    //P2.4-red, P1.5-green,  P1.4-blue
    P2DIR |= BIT4;
    P1DIR |= BIT4 | BIT5;
    P2OUT &= ~(BIT4);
    P1OUT &= ~(BIT4 | BIT5);
}

void Toggle_LED(void) {
    P2OUT ^= BIT4;
}

void Toggle_Buzzer(void) {
    if (state_button_up()) {
        P2OUT ^= BIT5;
    }
}


