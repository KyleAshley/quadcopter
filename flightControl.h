/********************************************************/
// Flight Control
// AUTHOR: Kyle Ashley
// Data and functions combining sensor output
/********************************************************/
#pragma once
#ifndef FLIGHTCONTROL_H_
#define FLIGHTCONTROL_H_

/*******************************************************************************/
// First three must be done in order
/*******************************************************************************/
void fc_initialize_motors(void);
int** fc_get_motors(int);
void fc_correct(int, int**);

void fc_adjustDuty(int, int);
int fc_isNominal(int, float);
float fc_angle_off(int);
long fc_getCurrentPWMDTY(int);
float fc_gainOf(int);



#endif
