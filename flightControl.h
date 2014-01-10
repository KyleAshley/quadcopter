/********************************************************/
// Flight Control
// AUTHOR: Kyle Ashley
// Data and functions combining sensor output
/********************************************************/
#pragma once
#ifndef FLIGHTCONTROL_H_
#define FLIGHTCONTROL_H_

#include "bmp.h"
#include "accel.h"
#include "mag.h"

#define ALTTHRESH .4		// meters
#define HEADINGTHRESH 2.0	//degrees
#define PITCHTHRESH 2.0		// degrees
#define ROLLTHRESH 2.0		// degrees

#define COMP_COEF 5
#define BR 0x1F                 // 100kHz BR

// Flight Control Variables
/******************************************************************************/
extern float fc_actualPitch;
extern float fc_desiredPitch;

extern float fc_actualRoll;
extern float fc_desiredRoll;

extern float fc_actualAltitude;
extern float fc_desiredAltitude;

extern float fc_actualHeading;
extern float fc_desiredHeading;


// Flight Control Functions
/******************************************************************************/
void fc_setup(void);                       // sets up all sensors and initializes navigation

//Mid Level Utility
/****************************************************/
int fc_isHoldingPitch(void);            // compares actual and desired pitches
int fc_isHoldingRoll(void);				// compares actual and desired rolls
int fc_isHoldingAltitude(void);         // comapres actual and desired altitudes
int fc_isHoldingHeading(void);			// compares actual and desired headings

void fc_update(void);
//Lower Level Utility
/*****************************************************/
void fc_capturePitch(void);                 // updates actual pitch
void fc_captureRoll(void);					// updates actual roll
void fc_captureAltitude(void);              // updates actual altitude
void fc_captureHeading(void);				// updates actual heading

void fc_setPitch(float);
void fc_setRoll(float);
void fc_setAltitude(float);
void fc_setHeading(float);

#endif
