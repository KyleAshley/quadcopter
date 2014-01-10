/*****************************************************/
// Magnetometer Functions
// AUTHOR: Kyle Ashley
/*****************************************************/
#pragma once
#include <math.h>
#include "mag.h"
#include "i2c.h"
#include "delays.h"

// Device ID
#define m_ID 0x3C

// Device Control Registers
#define m_CONFIG1 0x00
#define m_CONFIG2 0x01
#define m_MODE 0x02
#define m_STAT 0x09
#define m_IDA 0x0A
#define m_IDB 0x0B
#define m_IDC 0x0C

// Device Result Registers
#define m_xH 0x03
#define m_xL 0x04
#define m_zH 0x05
#define m_zL 0x06
#define m_yH 0x07
#define m_yL 0x08

// Measurement Modes
#define M_CONTINUOUS 0x00
#define M_SINGLESHOT 0x01
#define M_IDLE 0x10

// Declination Angle
#define M_DECLINATION -0.112

#define M_PI 3.14159265358

float m_xRaw;        // raw sensor reads
float m_yRaw;
float m_zRaw;

float m_scale;       // scalar for raw mag readings
float m_heading;     // calculated tilt-corrected heading (Deg)

/******************************************************************************/
// PRE: iic communication has been initiated
// POST: Initializes the mag with the given gauss scale
/******************************************************************************/
void m_setup(float gauss)
{
  unsigned int regValue = 0x00;
	I2C_Write_Byte(m_ID, m_CONFIG1, 0x70);

  if(gauss == 0.88)
  {
      regValue = 0x00;
      m_scale = 0.73;
  }
  else if(gauss == 1.3)
  {
      regValue = 0x20;
      m_scale = 0.92;
  }
  else if(gauss == 1.9)
  {
      regValue = 0x40;
      m_scale = 1.22;
  }
  else if(gauss == 2.5)
  {
      regValue = 0x60;
      m_scale = 1.52;
  }
  else if(gauss == 4.0)
  {
      regValue = 0x80;
      m_scale = 2.27;
  }
  else if(gauss == 4.7)
  {
      regValue = 0xA0;
      m_scale = 2.56;
  }
  else if(gauss == 5.6)
  {
      regValue = 0xC0;
      m_scale = 3.03;
  }
  else if(gauss == 8.1)
  {
      regValue = 0xE0;
      m_scale = 4.35;
  }
  else
  {
    regValue = 0xA0;
    m_scale = 2.56;
  }

   // Setting is in the top 3 bits of the register.
  I2C_Write_Byte(m_ID, m_CONFIG2, regValue);

  // Set Continuous mode
  I2C_Write_Byte(m_ID, m_MODE, M_CONTINUOUS);

	LCDDelayDATA(600);
}


/******************************************************************************/
// PRE: iic communication has been initiated
// POST: Sets the measurement mode of the mag, either continuous or single shot
/******************************************************************************/
void m_setMeasurementMode(unsigned int mode)
{
   I2C_Write_Byte(m_ID, m_MODE, mode);
}

/******************************************************************************/
// PRE: iic communication has been initiated
// POST: Requests read and updates raw X value from mag
/******************************************************************************/
void m_updateXRaw()
{
	float x;
	unsigned char xH, xL;
	xH = I2C_Read_Byte(m_ID, m_xH);
	xL = I2C_Read_Byte(m_ID, m_xL);
	x = ((xH << 8) | xL);
	m_xRaw = x * m_scale;
}

/******************************************************************************/
// PRE: iic communication has been initiated
// POST: Requests read and updates raw Y value from mag
/******************************************************************************/
void m_updateYRaw()
{
	float y;
	unsigned char yH, yL;
	yH = I2C_Read_Byte(m_ID, m_yH);
	yL = I2C_Read_Byte(m_ID, m_yL);
	y = ((yH << 8) | yL);
	m_yRaw = y * m_scale;
}

/******************************************************************************/
// PRE: iic communication has been initiated
// POST: Requests read and updates raw Z value from mag
/******************************************************************************/
void m_updateZRaw()
{
	float z;
	unsigned char zH, zL;
	zH = I2C_Read_Byte(m_ID, m_zH);
	zL = I2C_Read_Byte(m_ID, m_zL);
	z = ((zH << 8) | zL);
	m_zRaw = z * m_scale;
}

/******************************************************************************/
// PRE: X, Y, Z raw values have been read and updated
// POST: Calculates NON-TILTCORRECTED heading and updates m_heading
/******************************************************************************/
void m_updateHeading(void)
{
    float heading;
    heading = atan2(m_xRaw, m_yRaw);

    // correct for angle of declination
    heading += M_DECLINATION;

    // Correct for when signs are reversed.
    if(heading < 0)
    heading += 2*M_PI;

    // Check for wrap due to addition of declination.
    if(heading > 2*M_PI)
    heading -= 2*M_PI;

    //Convert radians to degrees for readability.
    heading = heading * 180 / M_PI;
    m_heading = heading;
}

/******************************************************************************/
// PRE: X, Y, Z raw values have been read and updated
// POST: Calculates TILTCORRECTED heading and updates m_heading
/******************************************************************************/
void m_updateTiltHeading(float pitch, float roll)
{
    float cosRoll, sinRoll, cosPitch, sinPitch, Xh, Yh;
    float pitchRad, rollRad, heading;

    pitchRad = (pitch * M_PI) / 180;
    rollRad = (roll * M_PI) / 180;

    // We cannot correct for tilt over 40 degrees with this algorithm, if the board is tilted as such, return the uncorrected heading.
    if(rollRad > 0.78 || rollRad < -0.78 || pitchRad > 0.78 || pitchRad < -0.78)
    {
      m_updateHeading();
      return;
    }

    else
    {
        // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
        cosRoll = cos(rollRad);
        sinRoll = sin(rollRad);
        cosPitch = cos(pitchRad);
        sinPitch = sin(pitchRad);

        Yh = (m_xRaw*cosRoll) + (m_yRaw*sinPitch*sinRoll) - (m_zRaw*cosPitch*sinRoll);   // Tilt compensated Magnetic filed X:
        Xh = (m_yRaw*cosPitch) - (m_zRaw*sinPitch);                                   // Tilt compensated Magnetic filed Y:
        heading = atan2(Yh,Xh);

        heading += M_DECLINATION;

        // Correct for when signs are reversed.
        if(heading < 0)
         heading += 2*M_PI;

        // Check for wrap due to addition of declination.
        if(heading > 2*M_PI)
         heading -= 2*M_PI;

        //Convert radians to degrees for readability.
        heading = heading * 180 / M_PI;
        m_heading = heading;
  }
}


