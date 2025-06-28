#ifndef TIME_H_
#define TIME_H_
#include <stdint.h>

extern volatile uint8_t FLAG;
typedef enum {
    LED_RED,    // P2.4 (TA2.1)
    LED_GREEN,  // P1.5 (TA0.2)
    LED_BLUE    // P1.4 (TA0.1)
} LED_Channel;
 
void Init_timer_no_interrupts(void);
void Init_pwm_rgb(void);
int8_t pwm_led(LED_Channel channel, int8_t duty);
int8_t pwm_red_sync(int8_t duty);
void Timer_Delay_25ms(void);
void Run_timer_interrupt_25ms(void);
uint8_t get_and_clear_FLAG_T(void);

#endif

