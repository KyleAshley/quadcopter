/**********************************************/
// Accellerometer Functions
// AUTHOR: Kyle Ashley
/**********************************************/
#pragma once
#include <math.h>
#include <stdio.h>
#include "i2c.h"
#include "accel.h"
#include "lcd.h"
#include "delays.h"

// Device ID
#define a_ID 0xA6

// Device Control Register Addresses
#define a_THRESH_TAP 0x1D
#define a_OFSX 0x1E
#define a_OFSY 0x1F
#define a_OFSZ 0x20
#define a_DUR 0x21
#define a_LATENT 0x22
#define a_WINDOW 0x23
#define a_THRESH_ACT 0x24
#define a_THRESH_INACT 0x25
#define a_TIME_INACT 0x26
#define a_ACT_INACT_CTL 0x27
#define a_THRESH_FF 0x28
#define a_TIME_FF 0x29
#define a_TAP_AXES 0x2A
#define a_ACT_TAP_STATUS 0x2B
#define a_BW_RATE 0x2C
#define a_POWER_CTL 0x2D
#define a_INT_ENABLE 0x2E
#define a_INT_MAP 0x2F
#define a_INT_SOURCE 0x30
#define a_DATA_FORMAT 0x31
#define a_FIFO_CTL 0x38
#define a_FIFO_STATUS 0x39

// Device Result Register Addresses
#define a_DATAX0 0x32
#define a_DATAX1 0x33
#define a_DATAY0 0x34
#define a_DATAY1 0x35
#define a_DATAZ0 0x36
#define a_DATAZ1 0x37

#define M_PI 3.14159265358

/* Associated Sensor Variables */
// Raw Accelerometer Readings
float a_xRaw;
float a_yRaw;
float a_zRaw;

// Converted Roll and Pitch Angles
float a_roll;
float a_pitch;

/* BEGIN FUNCTIONS */
/******************************************************************************/
// PRE: NONE
// POST: Initializes the accelerometer for +/- 2g readings
/******************************************************************************/
void a_setup(void)
{
	I2C_Write_Byte(a_ID, a_POWER_CTL, 0x08);
	a_setRange(2);
}

/******************************************************************************/
// PRE: NONE
// POST: Sets sensitivity of the accelerometer for +/- 2g readings
/******************************************************************************/
void a_setRange(int range)
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

/******************************************************************************/
// PRE: NONE
// POST: Requests new raw X reading and updates corresponding variable
/******************************************************************************/
void a_updateXRaw(void)
{
	float x;
     	unsigned char xH, xL;
     	xH = I2C_Read_Byte(a_ID, a_DATAX1);
     	xL = I2C_Read_Byte(a_ID, a_DATAX0);
     	x = ((xH << 8) | xL);
     	a_xRaw = x;
}

/******************************************************************************/
// PRE: NONE
// POST: Requests new raw Y reading and updates corresponding variable
/******************************************************************************/
void a_updateYRaw(void)
{
	float y;
     	unsigned char yH, yL;
     	yH = I2C_Read_Byte(a_ID, a_DATAY1);
     	yL = I2C_Read_Byte(a_ID, a_DATAY0);
     	y = ((yH << 8) | yL);
     	a_yRaw = y;

}

/******************************************************************************/
// PRE: NONE
// POST: Requests new raw Z reading and updates corresponding variable
/******************************************************************************/
void a_updateZRaw(void)
{
	float z;
     	unsigned char zH, zL;
     	zH = I2C_Read_Byte(a_ID, a_DATAZ1);
	zL = I2C_Read_Byte(a_ID, a_DATAZ0);
     	z = ((zH << 8) | zL);
     	a_zRaw = z;
}

/******************************************************************************/
// PRE: Raw values have been read and updated from accelerometer
// POST: Converts current raw values into absolute pitch degrees and updates
// corresponding variable
/******************************************************************************/
void a_updatePitchDeg(void)
{
    	float pitch = -((atan2(-a_yRaw, a_zRaw)) * 180.0)/M_PI;
    	a_pitch = pitch;
}

/******************************************************************************/
// PRE: Raw values have been read and updated from accelerometer
// POST: Converts current raw values into absolute roll degrees and updates
// corresponding variable
/******************************************************************************/
void a_updateRollDeg(void)
{
  	float roll = ((atan2(a_xRaw, sqrt(a_yRaw * a_yRaw + a_zRaw * a_zRaw))) * 180.0)/M_PI;
  	a_roll = roll;
}


/******************************************************************************/
// PRE: NONE
// POST: Variable Average Filter for successive sensor readings
/******************************************************************************/
/*
double a_lpFilter(double val, double old_fVal, double ALPHA)
{
  double fVal = (val * ALPHA) + (old_fVal * (1.0 - ALPHA));
  return fVal;
}
*/
  
