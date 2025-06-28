#ifndef DIGITAL_H_
#define DIGITAL_H_
#include <stdint.h>

void Init_buttons (void);
uint8_t state_button_down (void); /* Return 1 if the button is pressed, 0 otherwise*/
uint8_t state_button_up (void); /* Return 1 if the button is pressed, 0 otherwise*/
void Init_rgb_buzzer_led(void);
void Init_GPIO(void);
void Toggle_LED(void);
void Toggle_Buzzer(void);

#endif
