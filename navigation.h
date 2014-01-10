/********************************************************/
// navigation.h
// AUTHOR: Kyle Ashley
// Data and functions combining sensor output
/********************************************************/
#pragma once
#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include "bmp.h"
#include "accel.h"
#include "mag.h"

#define ALTTHRESH .4		// meters
#define HEADINGTHRESH 2.0	//degrees
#define PITCHTHRESH 2.0		// degrees
#define ROLLTHRESH 2.0		// degrees

#define COMP_COEF 5
#define BR 0x1F                 // 100kHz BR

// Navigation Variables
/******************************************************************************/
extern float nav_actualPitch;
extern float nav_desiredPitch;
extern float nav_pitchDiff;

extern float nav_actualRoll;
extern float nav_desiredRoll;
extern float nav_rollDiff;

extern float nav_actualAltitude;
extern float nav_desiredAltitude;
extern float nav_altitudeDiff;

extern float nav_actualHeading;
extern float nav_desiredHeading;
extern float nav_headingDiff;

// Navigation Functions
/******************************************************************************/
void nav_setup(void);                       // sets up all sensors and initializes navigation

//Mid Level Utility
/****************************************************/
int nav_isHoldingPitch(void);               // compares actual and desired pitches
int nav_isHoldingRoll(void);				// compares actual and desired rolls
int nav_isHoldingAltitude(void);            // comapres actual and desired altitudes
int nav_isHoldingHeading(void);			    // compares actual and desired headings

void nav_update(void);
//Lower Level Utility
/*****************************************************/
void nav_capturePitch(void);                // updates actual pitch
void nav_captureRoll(void);					// updates actual roll
void nav_captureAltitude(void);             // updates actual altitude
void nav_captureHeading(void);		        // updates actual heading

void nav_setPitch(float);
void nav_setRoll(float);
void nav_setAltitude(float);
void nav_setHeading(float);

#endif
