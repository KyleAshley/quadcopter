/**********************************************/
// Distance Sensor functions
// AUTHOR: Kyle Ashley
// Description: For use with long range Sharp 
// distance sensor
/**********************************************/

#pragma once
#include "distance.h"
#include "derivative.h"      /* derivative-specific definitions */
#include <math.h>
#include "delays.h"

void setupDISTSENSE()
{
	ATD0CTL2 = 0x80;
	LCDDelayDATA(60);
	ATD0CTL3 =0x08;
	ATD0CTL4 = 0x05;
	ATD0CTL5 = 0xA2;
}

double DtoVIN(double d, int res)
{
  double vin;
  vin = ((d / (pow(2, res))) * (VRH - VRL)) + VRL;
  return vin;
}

double convertCM(double vin)
{
  double dist;
  //dist = 39.78828617 * pow(vin, 2) - 176.4290234 * vin + 208.9268541;
  dist = 58 * pow(vin,-1.104); 
  return dist;
}

double getATDCH0()
{
    unsigned char digH, digL;
    double dig;
    
    while(!(ATD0STAT0 & 0x80));  
    digH = ATD0DR0H;
    digL = ATD0DR0L;
    
    digH &= 0x03;
    dig = (digH << 8) | digL;
    return dig;
}

double constrainDist(double cm)
{
  double conDist;
  if(cm < 20.0)
     conDist = 20.0;
  else if(cm > 150.0)
    conDist = 150.0;
  else
    conDist = cm;
  
  return conDist;
}
