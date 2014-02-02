/**********************************************/
// Distance Sensor functions
// AUTHOR: Kyle Ashley
// for use with long range Sharp distance sensor
// (20-150cm)
/**********************************************/
#pragma once
#include <math.h>
#include "distanceSensor.h"
#include "derivative.h"      /* derivative-specific definitions */
#include "delays.h"
#include "atd.h"

float d_distance;

/******************************************************************************/
// PRE: NONE
// POST: Sets up a single ATD Conversion on CH 2
/******************************************************************************/
void d_setup(void)
{
    atd0_powerOn();
    delay_LCD_DATA(60);
    atd0_setFFC(1);
    atd0_setLength(1);
    atd0_setFifo(0);
    atd0_setResolution(10);
    atd0_setJustification('r');
    atd0_setScan(1);
    atd0_setMulti(0);
    atd0_setStart(2);
}

/******************************************************************************/
// PRE: NONE
// POST: Reads ATD Conversion from a given channel and sets the current distance
/******************************************************************************/
void d_readDistance_20_150(unsigned int ch)          // reads a distance sensor on the given ATD channel
{
    float dist, digi;
    
    digi = atd0_readChX(ch);
    dist = 9462/(digi - 16.92);
    dist = d_constrain_20_150(dist);
    d_distance = dist;
}

/******************************************************************************/
// PRE: NONE
// POST: Reads ATD Conversion from a given channel and sets the current distance
/******************************************************************************/
void d_readDistance_4_30(unsigned int ch)          // reads a distance sensor on the given ATD channel
{
    float dist, digi;
    
    digi = atd0_readChX(ch);
    dist = 2076/(digi - 11);
    dist = d_constrain_4_30(dist);
    d_distance = dist;
}

/******************************************************************************/
// PRE: ATD Conversion has taken place and new data is ready to be interpretted
// POST: Converts a given digital value at a given resolution to a corresponding
// input voltage
/******************************************************************************/
float d_DtoVIN(float d, int res)
{
    float vin;
    vin = ((d / (pow(2, res))) * (VRH - VRL)) + VRL;
    return vin;
}


/******************************************************************************/
// PRE: We have determined the distance
// POST: Returns the corresponding distance constrained to readable range
/******************************************************************************/
float d_constrain_20_150(float cm)
{
    float conDist;
    if(cm < 20.0)
        conDist = 20.0;
    else if(cm > 150.0)
        conDist = 150.0;
    else
        conDist = cm;

    return conDist;
}

/******************************************************************************/
// PRE: We have determined the distance
// POST: Returns the corresponding distance constrained to readable range
/******************************************************************************/
float d_constrain_4_30(float cm)
{
    float conDist;
    if(cm < 4.0)
        conDist = 4.0;
    else if(cm > 30.0)
        conDist = 30.0;
    else
        conDist = cm;

    return conDist;
}

