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
int** fc_get_motors(const int);
void fc_correct(const int, const int**);

void fc_adjustDTY(int, int );
int fc_isNominal(const int, float );
float fc_angle_off(const int);
long fc_getCurrentPWMDTY(int);
float fc_gainOf(const int);



#endif
