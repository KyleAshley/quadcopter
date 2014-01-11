/********************************************************/
// Flight Control
// AUTHOR: Kyle Ashley
// Data and functions combining sensor output
/********************************************************/
#include <stdlib.h>
#include "flightControl.h"
#include "mag.h"
#include "accel.h"
#include "bmp.h"
#include "i2c.h"
#include "lcd.h"
#include "delays.h"
#include "distanceSensor.h"
#include "derivative.h"

/* Define:
 *     angle_off(ENUM TYPE) - ENUM TYPE 0=PITCH, 1=ROLL, 2=HEADING, 3=ALTITUDE
 *     unsigned int* motorRelationships(ENUM TYPE); // Returns Doubleton (int num_in_array, unsigned int* array)
 *     change_threshold =
 *
 *      Quad Setup:             0
 *                              |
 *                              |
 *   Forward Heading ^    3 ----X---- 1
 *                              |
 *                              |
 *                              2
 */

#define PITCH     0
#define ROLL      1
#define ALTITUDE  2
#define HEADING   3

#define POSITIVE   1
#define NEGATIVE  -1

#define PITCHGAIN   1.5
#define ROLLGAIN    1.5
#define ALTITUDEGAIN    1.5
#define HEADINGGAIN     1.5

#ifndef ALTTHRESH
#define ALTTHRESH       0.2         // meters
#endif
#ifndef HEADINGTHRESH
#define HEADINGTHRESH   2.0         // degrees
#endif
#ifndef PITCHTHRESH
#define PITCHTHRESH     2.0         // degrees
#endif
#ifndef ROLLTHRESH
#define ROLLTHRESH      2.0         // degrees
#endif

#define MINDTY 6000
#define MAXDTY 12000

/*******************************************************************************/
// Initializes motors on PWM channels 1, 3, 5, and 7. Call this immediately
// after powerup
/*******************************************************************************/
void fc_initialize_motors()
{
    PWMCTL = 0xFC;    // all 16 bit channels
    PWMCLK = 0xFF;    // all scaled clocks
    PWMPOL = 0xFF;
    PWMSCLA = 1;
    PWMSCLB = 1;
    PWMPRCLK = 0x11;   // PRE = 2

    // channel 1
    PWMPER0 = 0x4E;    // 20000
    PWMPER1 = 0x20;
    PWMDTY0 = 0x17;    // 6000
    PWMDTY1 = 0x70;

    // channel 3
    PWMPER2 = 0x4E;    // 20000
    PWMPER3 = 0x20;
    PWMDTY2 = 0x17;    // 6000
    PWMDTY3 = 0x70;

    // channel 5
    PWMPER4 = 0x4E;    // 20000
    PWMPER5 = 0x20;
    PWMDTY4 = 0x17;    // 6000
    PWMDTY5 = 0x70;

    // channel 7
    PWMPER6 = 0x4E;    // 20000
    PWMPER7 = 0x20;
    PWMDTY6 = 0x17;    // 6000
    PWMDTY7 = 0x70;

    PWME |= 0xFF;      // enable all channels
    LCDDelaySCROLL(270);
}

/*******************************************************************************/
// Defines motor compensations in each of four cases, pitch, roll, heading and altitude
// where sensor readings differ from dersired value
/*******************************************************************************/
int** fc_get_motors(const int TYPE_CORRECTION)
{
  int **operands;
  int numMotors, i;
  
	if (TYPE_CORRECTION == PITCH) {
		  numMotors = 2;
  		operands = malloc(sizeof(int*) * numMotors);
  		for(i = 0; i<numMotors; i++){  
      			operands[i] = malloc(sizeof(int) * 2);
    	}
  		operands[0][0] = 0;              // positive correction of motor 0
  		operands[0][1] = POSITIVE;
  		operands[1][0] = 2;              // negative correction of motor 2
  		operands[1][1] = NEGATIVE;       // important that operands[1][0][1] is inverse
	} else if (TYPE_CORRECTION == ROLL) {
    	numMotors = 2;
    	operands = malloc(sizeof(int*) * numMotors);
    	for(i = 0; i<numMotors; i++){  
        		operands[i] = malloc(sizeof(int) * 2);
      }
  		operands[0][0] = 1;              // positive correction of motor 1
  		operands[0][1] = POSITIVE;
  		operands[1][0] = 3;              // negative correction of motor 3
  		operands[1][1] = NEGATIVE;
  } else if (TYPE_CORRECTION == ALTITUDE) {
  		numMotors = 4;
    	operands = malloc(sizeof(int*) * numMotors);
    	for(i = 0; i<numMotors; i++){  
	 	  operands[i] = malloc(sizeof(int) * 2);
      }
  		operands[0][0] = 0;              // positive correction of motor 0
  		operands[0][1] = POSITIVE;
  		operands[1][0] = 2;              // positive correction of motor 2
  		operands[1][1] = POSITIVE;
  		operands[2][0] = 1;              // positive correction of motor 1
  		operands[2][1] = POSITIVE;
  		operands[3][0] = 3;              // positive correction of motor 3
  		operands[3][1] = POSITIVE;
	} else if (TYPE_CORRECTION == HEADING) {
  		numMotors = 4;
    	operands = malloc(sizeof(int*) * numMotors);
    	for(i = 0; i<numMotors; i++){  
        		operands[i] = malloc(sizeof(int) * 2);
      }
  		operands[0][0] = 0;              // positive correction of motor 0
  		operands[0][1] = POSITIVE;
  		operands[1][0] = 2;              // positive correction of motor 2
  		operands[1][1] = POSITIVE;
  		operands[2][0] = 1;              // negative correction of motor 1
  		operands[2][1] = NEGATIVE;
  		operands[3][0] = 3;              // negative correction of motor 3
  		operands[3][1] = NEGATIVE;
	}
 	return operands;
}

/*******************************************************************************/
// Main functionality of Flight Control:
// checks desired DOF against desired values in Nav and corrects PWMDTY of
// appropriate ESC Inputs.
/*******************************************************************************/
void fc_correct(int TYPE_CORRECTION, int** operands)
{
	int i, adjustment_value, numMotors;
	float theta;
	
	static int pitchGain[40] = 1;
  static int rollGain[40] = 1;
  static int altitudeGain[40] = 1;
  static int headingGain[40] = 1;
	
	numMotors = sizeof(operands) / (sizeof(int) * 2);

	// get Degrees off in Pitch, Roll etc.
	theta = fc_angle_off(TYPE_CORRECTION);

    	// do nothing if nominal
	if (fc_isNominal(TYPE_CORRECTION, theta))
		return;

	for (i = 0;  i < numMotors; ++i) {

	// adjustment proportional to theta * GAIN
	adjustment_value =  (int)(theta * fc_gainOf(TYPE_CORRECTION)) * operands[i][1];

        // Over/Under throttle protection
        if((adjustment_value + fc_getCurrentPWMDTY(operands[i][0]) > MAXDTY) || (adjustment_value + fc_getCurrentPWMDTY(operands[i][0]) < MINDTY))
            adjustment_value = 0;

        // adjust DutyFactor of given channel accordingly
	fc_adjustDuty(operands[i][0], adjustment_value);
	}
}

/*******************************************************************************/
// Adjusts the PWMDTY of given channel by given value
// (each increment of "value" corresponds to a 0.5% increase in throttle
/*******************************************************************************/
void fc_adjustDuty(int pwmChannel, int value)
{
	if (pwmChannel == 0) 
	{
		PWMDTY0 += value / 512;
		PWMDTY1 += value % 512;
	}
	
	else if (pwmChannel == 1) 
	{
		PWMDTY2 += value /512;
		PWMDTY3 += value % 512;
	} 
	
	else if (pwmChannel == 2) 
	{
		PWMDTY4 += value / 512;
		PWMDTY5 += value % 512;
	} 
	
	else if (pwmChannel == 3) 
	{
		PWMDTY6 += value / 512;
		PWMDTY7 += value % 512;
	}
}

/*******************************************************************************/
// Returns true of desired angle is within appropriate values, false otherwise
/*******************************************************************************/
int fc_isNominal(int TYPE_CORRECTION, float theta)
{
    if(TYPE_CORRECTION == 0)
    {
        if(theta > PITCHTHRESH || theta < -PITCHTHRESH)
            return 0;
        else
            return 1;
    }
    else if(TYPE_CORRECTION == 1)
    {
        if(theta > ROLLTHRESH || theta < -ROLLTHRESH)
            return 0;
        else
            return 1;
    }
    else if(TYPE_CORRECTION == 2)
    {
        if(theta > ALTTHRESH || theta < -ALTTHRESH)
            return 0;
        else
            return 1;
    }
    else if(TYPE_CORRECTION == 3)
    {
        if(theta > HEADINGTHRESH || theta < -HEADINGTHRESH)
            return 0;
        else
            return 1;
    }
}

/*******************************************************************************/
// Returns difference in desired readings within Nav.
// (computed by nav_get<parameter>diff)
/*******************************************************************************/
float fc_angle_off(int TYPE_CORRECTION)
{
    if(TYPE_CORRECTION == 0)
        return nav_pitchDiff;
    else if(TYPE_CORRECTION == 1)
        return nav_rollDiff;
    else if(TYPE_CORRECTION == 2)
        return nav_altitudeDiff;
    else if(TYPE_CORRECTIOn == 3)
        return nav_headingDiff;
}

/*******************************************************************************/
// Returns current DTY of a given motor
/*******************************************************************************/
long fc_getCurrentPWMDTY(int pwmChannel)
{
    if(pwmChannel == 0)
        return (PWMDTY0 << 8 | PWMDTY1);
    else if(pwmChannel == 1)
        return (PWMDTY2 << 8 | PWMDTY3);
    else if(pwmChannel == 2)
        return (PWMDTY4 << 8 | PWMDTY5);
    else if(pwmChannel == 3)
        return (PWMDTY6 << 8 | PWMDTY7);
}

/*******************************************************************************/
// Returns the appropriate gain value of a given type of correction
/*******************************************************************************/
float fc_gainOf(int TYPE_CORRECTION)
{   
    if(TYPE_CORRECTION == 0)
        return PITCHGAIN;
    else if(TYPE_CORRECTION == 1)
        return ROLLLGAIN;
    else if(TYPE_CORRECTION == 2)
        return ALTITUDEGAIN;
    else if(TYPE_CORRECTIOn == 3)
        return HEADINGGAIM;
}
