/********************************************/
// Author: Kyle Ashley
// accel.h
// for HC9S12 and ADXL035
/********************************************/
#ifndef ACCEL_H_
#define ACCEL_H_

/* Associated Sensor Variables */
// Raw Accelerometer Readings
extern float a_xRaw;
extern float a_yRaw;
extern float a_zRaw;

// Converted Roll and Pitch Angles
extern float a_roll;
extern float a_pitch;

// ACCEL Associated Functions
/******************************************************************************/
// sensor initialization
void a_setup(void);             // initilizes sensor with default range +/-2g
void a_setRange(int);           // set sensor sensitivity 2/4/8/16g

// sensor reads
void a_updateXRaw(void);            // Updates raw X value with a new reading
void a_updateYRaw(void);            // Updates raw Y value with a new reading
void a_updateZRaw(void);            // Updates raw Z value with a new reading

// converted calculations to be done after raw sensor reads
void a_updatePitchDeg(void);    // Calculates and updates converted pitch (Deg)
void a_updateRollDeg(void);     // Calculates and updates converted roll (Deg)


//double a_lpFilter(double, double, double);

#endif
