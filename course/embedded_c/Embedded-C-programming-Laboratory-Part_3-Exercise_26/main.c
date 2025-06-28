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
#define WIDTH 130
#define HIGH 130 
#define RECT_SIZE 20
#define CIRCLE_SIZE 20

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    Init_clock();
    Init_display();
    Init_buttons();
    Init_pwm_rgb();
    Init_ADC12_accelerometer();
    Run_timer_interrupt_40ms();

    __enable_interrupt();

    write_str('6');

    uint16_t jxy_and_axyz_results[5];
    //center limit 10<rect_x<WIDTH-10  10<rect_y<HIGH-10
    static uint8_t rect_x = 130/2 - 10, rect_y = 130/2 - 10;
    static uint8_t circle_x = 130/2 - 10, circle_y = 130/2 - 10;
    uint8_t rate = 4096/(120-10);
    uint8_t last_x = rect_x;
    uint8_t last_y = rect_y;
    uint8_t last_cx = circle_x;
    uint8_t last_cy = circle_y;
    int8_t red_duty = 0;
    int8_t green_duty = 0;
    int8_t blue_duty = 0;

    while(1) {
        if (get_and_clear_flag_40ms()){
            Capture_accelerometer(jxy_and_axyz_results);
            rect_x = jxy_and_axyz_results[0]/rate + 10;     
            rect_y = (110 - jxy_and_axyz_results[1]/rate ) + 10; 
            circle_x = jxy_and_axyz_results[2]/rate + 10;
            circle_y = (110 - jxy_and_axyz_results[3]/rate ) + 10; 
            if (state_button_up()) {
                red_duty+=5;
                if(red_duty > 100) red_duty = 0;
            }
            if (state_button_down()) {
                green_duty+=5;
                if(green_duty > 100) green_duty = 0;
            }
            if (state_button_left()) {
                blue_duty+=5;
                if(blue_duty > 100) blue_duty = 0;
            }
            if (state_button_right()) {
                red_duty = 0;
                green_duty = 0;
                blue_duty = 0;
            }
            pwm_led(LED_RED, red_duty);
            pwm_led(LED_GREEN, green_duty);
            pwm_led(LED_BLUE, blue_duty);
        }
        if(rect_x != last_x || rect_x != last_y) {
            Filled_rectangle(last_x-RECT_SIZE/2, 19+last_x-RECT_SIZE/2, last_y-RECT_SIZE/2, 19+last_y-RECT_SIZE/2, GRAPHICS_COLOR_WHITE);
            Filled_rectangle(rect_x-RECT_SIZE/2, 19+rect_x-RECT_SIZE/2, rect_y-RECT_SIZE/2, 19+rect_y-RECT_SIZE/2, GRAPHICS_COLOR_ORANGE);
            last_x = rect_x;
            last_y = rect_y;
        }
        if(circle_x != last_cx || circle_y != last_cy) {
            Filled_circle(last_cx, last_cy, 20, GRAPHICS_COLOR_WHITE);
            Filled_circle(circle_x, circle_y, 20, GRAPHICS_COLOR_GREEN);
            last_cx = circle_x;
            last_cy = circle_y;
        }
        write_str('6');
    }
}

