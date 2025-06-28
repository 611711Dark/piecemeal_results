#ifndef TIME_H_
#define TIME_H_
#include <stdint.h>

extern volatile uint8_t FLAG;
void Init_timer_no_interrupts(void);
void Init_pwm_red(void);
void Timer_Delay_25ms(void);
void Run_timer_interrupt_25ms(void);
uint8_t get_and_clear_FLAG_T(void);

#endif

