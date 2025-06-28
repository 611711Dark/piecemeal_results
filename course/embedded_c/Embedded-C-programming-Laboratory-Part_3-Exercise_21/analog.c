#include <msp430.h>
#include "analog.h"
void Init_ADC12(void) {
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = ADC12SHT0_10 | ADC12SHT0_9 | ADC12ON;//128 ADC12CLK cycles
    ADC12CTL1 = ADC12SHP;
    ADC12CTL0 |= ADC12ENC;
}

void Init_ADC12_joystick(void) {
    P6SEL |= BIT3 | BIT5;

    ADC12CTL0 &= ~ADC12ENC;

    ADC12CTL0 = ADC12SHT0_10 | ADC12SHT0_9 | ADC12ON | ADC12MSC;//128 ADC12CLK cycles
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;

    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_5; // jx
    ADC12MCTL1 = ADC12SREF_0 | ADC12INCH_3 | ADC12EOS; // jy

    ADC12CTL0 |= ADC12ENC;
}

void Capture_joystick(uint16_t results[2]) {
    ADC12CTL0 &= ~ADC12SC; 
    ADC12CTL0 |= ADC12SC;
    while (!(ADC12IFG & BIT1));
    
    results[0] = ADC12MEM0; // X
    results[1] = ADC12MEM1; // Y
    ADC12IFG &= ~BIT1; 
}

uint16_t Capture_11(void) {
    ADC12CTL0 &= ~ADC12ENC;
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_11;
    ADC12CTL0 |= ADC12ENC;
    
    ADC12CTL0 |= ADC12SC;
    while (!(ADC12IFG & BIT0)); 
     
    return ADC12MEM0;
}

uint16_t Capture_jx(void) {
    ADC12CTL0 &= ~ADC12ENC;          
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_5;        
    ADC12CTL0 |= ADC12ENC;           
    
    ADC12CTL0 |= ADC12SC;            
    while (!(ADC12IFG & BIT0));             
    ADC12IFG &= ~BIT0;               
    return ADC12MEM0;
}

uint16_t Capture_jy(void) {
    ADC12CTL0 &= ~ADC12ENC;        
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_3;     
    ADC12CTL0 |= ADC12ENC;         
    
    ADC12CTL0 |= ADC12SC;           
    while (!(ADC12IFG & BIT0));         
    ADC12IFG &= ~BIT0;              
    return ADC12MEM0;
}

float convert_to_volts(uint16_t data) {
    return (data * 3.3f) / 4095.0f;
}
