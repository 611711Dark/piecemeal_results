#include <msp430.h>
#include "analog.h"
void Init_ADC12(void) {
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = ADC12SHT0_10 | ADC12SHT0_9 | ADC12ON;//128 ADC12CLK cycles
    ADC12CTL1 = ADC12SHP;
    ADC12CTL0 |= ADC12ENC;
}

void Init_ADC12_joystick(void) {
    P6SEL |= BIT3;
    P6SEL |= BIT5; 
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = ADC12SHT0_10 | ADC12SHT0_9 | ADC12ON;//128 ADC12CLK cycles
    ADC12CTL1 = ADC12SHP;
    ADC12CTL0 |= ADC12ENC;
}

uint16_t Capture_11(void) {
    ADC12CTL0 &= ~ADC12ENC;
    ADC12MCTL0 = ADC12SREF_0 | ADC12INCH_11;
    ADC12CTL0 |= ADC12ENC;
    
    ADC12CTL0 |= ADC12SC;
    while (!(ADC12IFG & BIT0)); 
    ADC12IFG &= ~BIT0;  
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

