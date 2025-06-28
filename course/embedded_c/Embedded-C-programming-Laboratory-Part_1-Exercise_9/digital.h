#ifndef DIGITAL_H_
#define DIGITAL_H_
#include <stdint.h>

void Init_buttons (void);
void Init_buttons_interrupt(void);
uint8_t state_button_L (void); /* Return 1 if the button is pressed, 0 otherwise*/
uint8_t state_button_R (void); /* Return 1 if the button is pressed, 0 otherwise*/
uint8_t get_and_clear_FLAG_L(void);
uint8_t get_and_clear_FLAG_R(void);
void Init_rgb_led(void);
void round_robin_led(void);

#endif
