/*
 * game.h
 *
 *  Created on: 3 may. 2021
 *      Author: tonyb
 */

#ifndef GAME_H_
#define GAME_H_

#define CLOCK 24000000
#define NUM_OPTIONS 3

uint8_t check_SysTick_flag(void);
void Init_Game (void);
void Create_Menu(void);
uint8_t Show_Menu(uint8_t option);
void Set_Car_Color (uint8_t colour);
void Draw_Car (void);
void Draw_Road(void);
void Steering_Wheel (int8_t offset);
uint8_t change_speed(int8_t units);
uint8_t foo(void);
void show_error(int8_t *l1, int8_t *l2);



#endif /* GAME_H_ */
