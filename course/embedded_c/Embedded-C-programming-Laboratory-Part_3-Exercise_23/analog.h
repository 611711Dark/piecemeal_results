#ifndef ANALOG_H_
#define ANALOG_H_
#include <stdint.h>

void Init_ADC12(void);
void Init_ADC12_joystick(void);
void Capture_joystick(uint16_t results[2]);
void Init_ADC12_accelerometer(void);
void Capture_accelerometer(uint16_t results[5]);
uint16_t Capture_11(void);
uint16_t Capture_jx(void);
uint16_t Capture_jy(void);
float convert_to_volts(uint16_t data);

#endif
