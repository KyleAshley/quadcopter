/********************************************************/
// Flight Control
// AUTHOR: Kyle Ashley
// Data and functions combining sensor output
/********************************************************/
#include "flightControl.h"
#include "mag.h"
#include "accel.h"
#include "bmp.h"
#include "i2c.h"
#include "lcd.h"
#include "delays.h"
#include "distanceSensor.h"

#define ALTITUDETOLERANCE 5		// meters
#define HEADINGTOLERANCE 6.0	//degrees
#define PITCHTOLERANCE 6.0		// degrees
#define ROLLTOLERANCE 6.0		// degrees

#define BR 0x1F                 // 100kHz BR

/******************************************************************************/
// Flight Control Variables
/******************************************************************************/
float fc_actualPitch;
float fc_desiredPitch;

float fc_actualRoll;
float fc_desiredRoll;

float fc_actualAltitude;
float fc_desiredAltitude;

float fc_actualHeading;
float fc_desiredHeading;

/******************************************************************************/
// Flight Control Functions
/******************************************************************************/
void fc_setup(void)
{
    openI2C(BR);
    LCDDelayDATA(TCHAR);

    a_setup();
  	LCDDelayDATA(TCHAR);

  	b_setup();
  	LCDDelayDATA(TCHAR);

    m_setup(4.7);
  	LCDDelayDATA(TCHAR);

  	d_setup();
  	LCDDelayDATA(TCHAR);
}

// navi checks
/**********************************************************************************************************************/
int fc_isHoldingPitch(void)
{
	// nominal pitch
	if((fc_actualPitch > (fc_desiredPitch - PITCHTOLERANCE)) && (fc_actualPitch < (fc_desiredPitch + PITCHTOLERANCE)))
	{
		return 1;
	}
	// need positive compensation
	else
	{
		return 0;
	}
}

int fc_isHoldingRoll(void)
{
	// nominal roll
	if((fc_actualRoll > (fc_desiredRoll - ROLLTOLERANCE)) && (fc_actualRoll < (fc_desiredRoll + ROLLTOLERANCE)))
	{
		return 1;
	}
	// need positive compensation
	else
	{
		return 0;
	}
}

int fc_isHoldingAltitude(void)
{
	// nominal alt
	if((fc_actualAltitude > (fc_desiredAltitude - ALTITUDETOLERANCE)) && (fc_actualAltitude < (fc_desiredAltitude + ALTITUDETOLERANCE)))
	{
		return 1;
	}
	// need positive compensation
	else
	{
		return 0;
	}
}

int fc_isHoldingHeading(void)
{
	// nominal heading
	if((fc_actualHeading > (fc_desiredHeading - HEADINGTOLERANCE)) && (fc_actualHeading < (fc_desiredHeading + HEADINGTOLERANCE)))
	{
	  return 1;
	}
	// need positive compensation
	else
	{
	  	return 0;
	}
}

void fc_update(void)
{
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();
    a_updatePitchDeg();
    a_updateRollDeg();
    fc_actualPitch = a_pitch;
    fc_actualRoll = a_roll;
    
    b_updateTempC();
    b_updatePressure();
    b_updateAltitude();
    fc_actualAltitude = b_altitude;
    
    m_updateXRaw();
    m_updateYRaw();
    m_updateZRaw();

    m_updateTiltHeading(a_pitch, a_roll);
    fc_actualHeading = m_heading;
    
}


//Lower Level Utility
/*****************************************************/

void fc_capturePitch(void) 				// updates desired pitch with current pitch
{
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();

    a_updatePitchDeg();
    fc_actualPitch = a_pitch;
}

void fc_captureRoll(void)  				// updates desired roll with current roll
{
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();

    a_updateRollDeg();
    fc_actualRoll = a_roll;
}

void fc_captureAltitude(void)  				// updates desired roll with current roll
{
    b_updateTempC();
    b_updatePressure();
    b_updateAltitude();

    fc_actualAltitude = b_altitude;
}

void fc_captureHeading(void)
{
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();
    
    m_updateXRaw();
    m_updateYRaw();
    m_updateZRaw();

    m_updateTiltHeading(a_pitch, a_roll);
    fc_actualHeading = m_heading;
}

void fc_setPitch(float p)
{
    fc_desiredHeading = p;
}

void fc_setRoll(float r)
{
    fc_desiredRoll = r;
}

void fc_setAltitude(float a)
{
    fc_desiredAltitude = a;
}

void fc_setHeading(float h)
{
    fc_desiredHeading = h;
}
