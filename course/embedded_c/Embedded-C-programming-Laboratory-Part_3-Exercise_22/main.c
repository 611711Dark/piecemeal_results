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
    Init_ADC12_accelerometer();

    write_str('2');
    rectangle();
    circle();

    uint16_t jxy_and_axyz_results[5];
    
    while(1) {
        Capture_accelerometer(jxy_and_axyz_results);
        write_num_str(jxy_and_axyz_results[0],-24);
        write_num_str(jxy_and_axyz_results[1],-16);
        write_num_str(jxy_and_axyz_results[2],-8);
        write_num_str(jxy_and_axyz_results[3],0);
        write_num_str(jxy_and_axyz_results[4],8);
        write_voltage_str(convert_to_volts(jxy_and_axyz_results[0]),16);
        write_voltage_str(convert_to_volts(jxy_and_axyz_results[1]),24);
        __delay_cycles(100000);
    }
}

