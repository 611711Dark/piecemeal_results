/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include "driverlib.h"
#include "system.h"
#include "display.h"
#include <intrinsics.h>
#include <msp430.h> 
#include <stdint.h> 
#include <drawings.h>
#include <digital.h>
//******************************************************************************
//!
//!   Empty Project that includes driverlib
//!
//******************************************************************************

static uint8_t rectangle_state = 0;
static uint8_t circle_state = 0;

static uint16_t debounce_counter_L = 0;
static uint16_t debounce_counter_R = 0;
static uint8_t button_L_processed = 0;
static uint8_t button_R_processed = 0;

#define DEBOUNCE_DELAY 5000

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    Init_clock();
    Init_display();
    Init_buttons_interrupt();

    __enable_interrupt();

    write_str('7');
    
    while(1) {
        if(get_and_clear_FLAG_L()) {
            if(!button_L_processed) {
                rectangle_state = !rectangle_state;
                
                if(rectangle_state) {
                    rectangle();
                } else {
                    clear_drawing(1);
                }
                
                button_L_processed = 1;
                debounce_counter_L = 0;
            }
        }
        
        if(get_and_clear_FLAG_R()) {
            if(!button_R_processed) {
                circle_state = !circle_state;
                
                if(circle_state) {
                    circle();
                } else {
                    clear_drawing(2);
                }
                
                button_R_processed = 1;
                debounce_counter_R = 0;
            }
        }
        
        if(button_L_processed) {
            debounce_counter_L++;
            if(debounce_counter_L >= DEBOUNCE_DELAY) {
                button_L_processed = 0;
            }
        }
        
        if(button_R_processed) {
            debounce_counter_R++;
            if(debounce_counter_R >= DEBOUNCE_DELAY) {
                button_R_processed = 0;
            }
        }
        
        __delay_cycles(100);
    }
}

