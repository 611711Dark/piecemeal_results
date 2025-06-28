#ifndef ANALOG_H_
#define ANALOG_H_
#include <stdint.h>

void Init_ADC12(void);
void Init_ADC12_joystick(void);
uint16_t Capture_11(void);
uint16_t Capture_jx(void);
uint16_t Capture_jy(void);
void Capture_joystick(uint16_t results[2]);
float convert_to_volts(uint16_t data);

#endif
