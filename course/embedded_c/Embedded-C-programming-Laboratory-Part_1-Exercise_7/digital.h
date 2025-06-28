#ifndef DIGITAL_H_
#define DIGITAL_H_
#include <stdint.h>

extern volatile uint8_t FLAG_L;
extern volatile uint8_t FLAG_R;

void Init_buttons (void);
void Init_buttons_interrupt(void);
uint8_t state_button_L (void); /* Return 1 if the button is pressed, 0 otherwise*/
uint8_t state_button_R (void); /* Return 1 if the button is pressed, 0 otherwise*/


#endif
