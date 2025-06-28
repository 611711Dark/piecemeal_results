/*
 * system.h
 *
 *  Created on: 16 may. 2020
 *      Author: tonyb
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>
/********************************************/
/****  Set DCO as source clock at 8MHz   ****/
/****  MCLK & SMCLK 8MHz                 ****/
/****  ACLK REF0 32.768 kHz              ****/
/****                                    ****/
/********************************************/

void Init_clock();

/********************************************/
/****  Converts a 16-bits signed int     ****/
/****  to a 6-digits null-terminated     ****/
/****  string, 0s left padding           ****/
/****  WARNING. target must be 7 char    ****/
/****  length at least                   ****/
/****                                    ****/
/********************************************/
void itoa_7_null_terminated (int16_t dato, uint8_t *target);

#endif /* SYSTEM_H_ */
