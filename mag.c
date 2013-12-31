/*****************************************************/
// mag.c
// AUTHOR: Kyle Ashley
// Description: For use with HMC5883 magnetometer and 
// HCS12
/*****************************************************/
#pragma once
#include <math.h>
#include "mag.h"
#include "I2C.h"
#include "delays.h"

#define M_PI 3.14159265358

// initializes the mag
void setupMag(float scale)
{
	I2C_Write_Byte(m_ID, m_CONFIG1, 0x70);
	set_m_Scale(scale);
	set_m_MeasurementMode(0x00);

	LCDDelayDATA(600);
	
}

// sets the scale applied to sensor values
void set_m_Scale(float gauss)
{
    unsigned int regValue = 0x00;
    if(gauss == 0.88)
    {
        regValue = 0x00;
    }
    else if(gauss == 1.3)
    {
        regValue = 0x20;;
    }
    else if(gauss == 1.9)
    {
        regValue = 0x40;;
    }
    else if(gauss == 2.5)
    {
        regValue = 0x60;
    }
    else if(gauss == 4.0)
    {
        regValue = 0x80;
    }
    else if(gauss == 4.7)
    {
        regValue = 0xA0;
    }
    else if(gauss == 5.6)
    {
        regValue = 0xC0;
    }
    else if(gauss == 8.1)
    {
        regValue = 0xE0;
    }
    
    // Setting is in the top 3 bits of the register.
    I2C_Write_Byte(m_ID, m_CONFIG2, regValue);
}

// sets the measurement mode (continuous or single shot)
void set_m_MeasurementMode(unsigned int mode)
{
   I2C_Write_Byte(m_ID, m_MODE, mode);
}

// returns the raw x-mag value
double getMagX()
{
	double x;
	unsigned char xH, xL;
	xH = I2C_Read_Byte(m_ID, m_xH);
	xL = I2C_Read_Byte(m_ID, m_xL);
	x = ((xH << 8) | xL);
	return x;
}

// returns the raw y-mag value
double getMagY()
{
	double y;
	unsigned char yH, yL;
	yH = I2C_Read_Byte(m_ID, m_yH);
	yL = I2C_Read_Byte(m_ID, m_yL);
	y = ((yH << 8) | yL);
	return y;
}

// returns the raw z-mag value
double getMagZ()
{
	double z;
	unsigned char zH, zL;
	zH = I2C_Read_Byte(m_ID, m_zH);
	zL = I2C_Read_Byte(m_ID, m_zL);
	z = ((zH << 8) | zL);
	return z;
}

/////////////////////////////////////////////////
/*
double getMagRaw()
{
  unsigned char data[6];
  I2C_Read_Bytes(m_ID, m_xH, 6, buffer);
*/  
//////////////////////////////////////////////////  

// converts two x/y mag readings to corresponding angle
double getHeading(double x, double y)
{  
  double heading, headingDegrees, declinationAngle;
  heading = atan2(x, y);
 
  declinationAngle = -0.112;
  heading += declinationAngle;
 
  // Correct for when signs are reversed.
  if(heading < 0)
   heading += 2*M_PI;
  
  // Check for wrap due to addition of declination.
  if(heading > 2*M_PI)
   heading -= 2*M_PI;
 
  //Convert radians to degrees for readability.
  headingDegrees = heading * 180 / M_PI;
  return headingDegrees;
}

double getTiltHeading(double rollRadians, double pitchRadians, double mX, double mY, double mZ)
{  
  double cosRoll, sinRoll, cosPitch, sinPitch, Xh, Yh, heading, declinationAngle, headingDegrees;
  
  // We cannot correct for tilt over 40 degrees with this algorithm, if the board is tilted as such, return the uncorrected heading.
  if(rollRadians > 0.78 || rollRadians < -0.78 || pitchRadians > 0.78 || pitchRadians < -0.78)
  {
    return getHeading(mX, mY);
  } 
  
  else
  {
    // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
    cosRoll = cos(rollRadians);
    sinRoll = sin(rollRadians);  
    cosPitch = cos(pitchRadians);
    sinPitch = sin(pitchRadians);
    
    Yh = (mX*cosRoll) + (mY*sinPitch*sinRoll) - (mZ*cosPitch*sinRoll);   // Tilt compensated Magnetic filed X:
    Xh = (mY*cosPitch) - (mZ*sinPitch);                                   // Tilt compensated Magnetic filed Y:
    heading = atan2(Yh,Xh);                      
    
    declinationAngle = -0.112;
    heading += declinationAngle;
   
    // Correct for when signs are reversed.
    if(heading < 0)
     heading += 2*M_PI;
    
    // Check for wrap due to addition of declination.
    if(heading > 2*M_PI)
     heading -= 2*M_PI;
   
    //Convert radians to degrees for readability.
    headingDegrees = heading * 180 / M_PI;
    return headingDegrees;
  }
}
