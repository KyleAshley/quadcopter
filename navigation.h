/********************************************************/
// navigation.h
// AUTHOR: Kyle Ashley
// Data and functions combining sensor output
/********************************************************/
#pragma once
#ifndef NAVIGATION_H_
#define NAVIGATION_H_

// Navigation Variables
// 2 element buffers for sensor readings. buffers[0] contain most recent
// buffers [1] contain previous read
/******************************************************************************/
extern float nav_actualPitch[2];
extern float nav_desiredPitch;
extern float nav_pitchDiff;

extern float nav_actualRoll[2];
extern float nav_desiredRoll;
extern float nav_rollDiff;

extern float nav_actualAltitude[2];
extern float nav_desiredAltitude;
extern float nav_altitudeDiff;

extern float nav_actualHeading[2];
extern float nav_desiredHeading;
extern float nav_headingDiff;

// Navigation Functions
/******************************************************************************/
void nav_setup(void);                       // sets up all sensors and initializes navigation

//void nav_update(void);

// Lower Level Utility
/*****************************************************/
void nav_capturePitch(void);                // updates actual pitch
void nav_captureRoll(void);					        // updates actual roll
void nav_captureAltitude(void);             // updates actual altitude
void nav_captureHeading(void);		          // updates actual heading

void nav_setPitch(float);                   // sets desired pitch
void nav_setRoll(float);                    // sets desired roll
void nav_setAltitude(float);                // sets desired altitude
void nav_setHeading(float);                 // sets desired heading

#endif
