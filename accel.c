/**********************************************/
// Accellerometer Functions
// AUTHOR: Kyle Ashley
/**********************************************/
#pragma once
#include <math.h>
#include <stdio.h>
#include "I2C.h"
#include "accel.h"
#include "LCD_write.h"
#include "delays.h"

#define M_PI 3.14159265358

// initializes the accelerometer
void setupAccel()
{
	I2C_Write_Byte(a_ID, a_POWER_CTL, 0x08);
}

// sets the range of sensor values
void setAccelRange(int range)
{
	switch(range)
	{
		case 16: 
			I2C_Write_Byte(a_ID, a_DATA_FORMAT, 0x03);
			break;
		case 8: 
			I2C_Write_Byte(a_ID, a_DATA_FORMAT, 0x02);
			break;
		case 4: 
			I2C_Write_Byte(a_ID, a_DATA_FORMAT, 0x01);
			break;
		case 2: 
			I2C_Write_Byte(a_ID, a_DATA_FORMAT, 0x00);
			break;
	}
}

// raw X value from accelerometer
double getAccelX()
{
	   double x;
     unsigned char xH, xL;
     xH = I2C_Read_Byte(a_ID, a_DATAX1);
     xL = I2C_Read_Byte(a_ID, a_DATAX0);
     x = ((xH << 8) | xL);
     return x;
}

// raw Y value from accelerometer
double getAccelY()
{
	   double y;
     unsigned char yH, yL;
     yH = I2C_Read_Byte(a_ID, a_DATAY1);
     yL = I2C_Read_Byte(a_ID, a_DATAY0);
     y = ((yH << 8) | yL);
     return y;

}

// raw Z value from accelerometer
double getAccelZ()
{
	   double z;
     unsigned char zH, zL;
     zH = I2C_Read_Byte(a_ID, a_DATAZ1);
     zL = I2C_Read_Byte(a_ID, a_DATAZ0);
     z = ((zH << 8) | zL);
     return z;
}

// returns the Pitch in Degrees
double getAccelPitchDeg(double faY, double faZ)
{
    double pitch = -(atan2(-faY, faZ) * 180.0)/M_PI;
    return pitch;
}

// returns the Roll in Degrees
double getAccelRollDeg(double faX, double faY, double faZ) 
{
  double roll = (atan2(faX, sqrt(faY * faY + faZ * faZ)) * 180.0)/M_PI;
  return roll;
}

double getAccelPitchRad(double faY, double faZ)
{
    double pitch = -(atan2(-faY, faZ));
    return pitch;
}

// returns the Roll in Degrees
double getAccelRollRad(double faX, double faY, double faZ) 
{
  double roll = (atan2(faX, sqrt(faY * faY + faZ * faZ)));
  return roll;
}

// filters a value through a Low Pass Filter
double lpFilter(double val, double old_fVal, double ALPHA)
{
  double fVal = (val * ALPHA) + (old_fVal * (1.0 - ALPHA));
  return fVal;
}

// prints three raw accleration values to the LCD
void output_accelXYZ(double ax, double ay, double az)
{
      unsigned char strx[5];
      unsigned char stry[5];
      unsigned char strz[5];
      unsigned int cx, cy, cz;
      unsigned int ctr;
      
      cx = 0;
      cy = 0;
      cz = 0;

      sprintf(strx,"%5.2f",ax);        // print x
      while (strx[cx] != '\0') 
  	  {    
        DATWRTFIRST(strx[cx]);
        LCDDelayDATA(DAT);
        cx++;
      }
        
      DATWRTFIRST(' ');             // print y on first line after x
      sprintf(stry,"%5.2f",ay);
      while (stry[cy] != '\0') 
    	{    
        DATWRTFIRST(stry[cy]);
        LCDDelayDATA(DAT);
        cy++;
      }
      
      COMWRTFIRST(0xC0);
                                     // skip a line and print z
      sprintf(strz,"%5.2f",az);
      while (strz[cz] != '\0') 
    	{    
        DATWRTFIRST(strz[cz]);
        LCDDelayDATA(DAT);
        cz++;
      }
        
      for(ctr = 0; ctr < 5; ctr++)
      { 
        LCDDelayDATA(TCHAR * 8);
      }
      COMWRTFIRST(0x01);
      LCDDelayDATA(TCHAR); 
}

// prints decimal angle value to the LCD
void output_accelANGLE(double angle)
{  
    unsigned int cR = 0;
    unsigned char strAngle[6];
    
    sprintf(strAngle,"%3.1f",angle);
      while (strAngle[cR] != '\0') 
  	  {    
        DATWRTFIRST(strAngle[cR]);
        LCDDelayDATA(DAT);
        cR++;
      }
}

double constrainRad(double rad)
{
  if(rad < 0)
     rad += 2*M_PI;
    
  // Check for wrap due to addition of declination.
  else if(rad > 2*M_PI)
     rad -= 2*M_PI;
  
  return rad;
}
  
