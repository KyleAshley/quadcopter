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
void d_readDistance(unsigned int ch)          // reads a distance sensor on the given ATD channel
{
    float dist, vin, digi;
    
    digi = atd0_readChX(ch);
    vin = d_DtoVIN(digi, 10);
    dist = d_convertCM(vin);
    dist = d_constrainDist(dist);
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
// PRE: ATD Conversion has taken place and new data has been converted to its VIN
// POST: Returns the corresponding distance to any given input voltage
/******************************************************************************/
float d_convertCM(float vin)
{
    float dist;
    //dist = 39.78828617 * pow(vin, 2) - 176.4290234 * vin + 208.9268541;
    dist = 58 * pow(vin,-1.104);
    return dist;
}

/******************************************************************************/
// PRE: We have determined the distance
// POST: Returns the corresponding distance constrained to readable range
/******************************************************************************/
float d_constrainDist(float cm)
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
