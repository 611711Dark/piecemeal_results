#ifndef ANALOG_H_
#define ANALOG_H_
#include <stdint.h>

void Init_ADC12(void);
void Init_ADC12_joystick(void);
uint16_t Capture_11(void);
uint16_t Capture_jx(void);
uint16_t Capture_jy(void);

#endif
