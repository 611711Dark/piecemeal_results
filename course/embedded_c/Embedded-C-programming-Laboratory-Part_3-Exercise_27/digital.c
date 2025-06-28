#include <msp430.h>
#include "digital.h"
#include <stdint.h>
#include <drawings.h>
#include <intrinsics.h> 

static volatile uint8_t FLAG_L = 0;
static volatile uint8_t FLAG_R = 0;

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

void Init_buttons_interrupt(void) {
    Init_buttons();
    
    // falling edge）
    P2IES |= BIT1;
    P2IE |= BIT1;
    
    P1IES |= BIT1;
    P1IE |= BIT1;
}

uint8_t state_button_L(void) {
    return (P2IN & BIT1) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t state_button_R(void) {
    return (P1IN & BIT1) ? 0 : 1; // Return 1 if the button is pressed
}

uint8_t get_and_clear_FLAG_L(void) {
    uint16_t GIE_state = __get_SR_register() & GIE;
    __disable_interrupt();
    
    uint8_t temp = FLAG_L; 
    FLAG_L = 0;
    
    __bis_SR_register(GIE_state); // recover
    return temp;
}


uint8_t get_and_clear_FLAG_R(void) {
    uint16_t GIE_state = __get_SR_register() & GIE; 
    __disable_interrupt();
    
    uint8_t temp = FLAG_R;
    FLAG_R = 0;
    
    __bis_SR_register(GIE_state); // recover
    return temp;
}




#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void) {
     if(P2IFG & BIT1) {
        FLAG_L = 1;
        P2IFG &= ~BIT1;
    }
}
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void) {
    if(P1IFG & BIT1) {
        FLAG_R = 1;
        P1IFG &= ~BIT1;
    }
}
