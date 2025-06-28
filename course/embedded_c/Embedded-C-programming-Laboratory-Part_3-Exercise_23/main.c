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
#include "drawings.h"
#include "digital.h"
#include "analog.h"
#include "time.h"
#include <intrinsics.h>
//******************************************************************************
//!
//!   Empty Project that includes driverlib
//!
//******************************************************************************
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    Init_clock();
    Init_display();
    Init_buttons();
    Init_pwm_rgb();

    write_str('3');
    rectangle();
    circle();

    int8_t red_duty = 0;
    int8_t green_duty = 0;
    int8_t blue_duty = 0;

    while(1) {
        if (state_button_up()) {
            red_duty++;
        }
        if (state_button_down()) {
            green_duty++;
        }
        if (state_button_left()) {
            blue_duty++;
        }
        if (state_button_right()) {
            red_duty = 0;
            green_duty = 0;
            blue_duty = 0;
        }
        pwm_led(LED_RED, red_duty);
        pwm_led(LED_GREEN, green_duty);
        pwm_led(LED_BLUE, blue_duty);
        write_num_str(red_duty,-24);
        write_num_str(green_duty,-16);
        write_num_str(blue_duty,-8);

        __delay_cycles(100000);
    }
}

