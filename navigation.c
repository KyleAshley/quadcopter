#include "navigation.h"
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
float nav_actualPitch;
float nav_desiredPitch;
float nav_pitchDiff;

float nav_actualRoll;
float nav_desiredRoll;
float nav_rollDiff;

float nav_actualAltitude;
float nav_desiredAltitude;
float nav_altitudeDiff;

float nav_actualHeading;
float nav_desiredHeading;
float nav_headingDiff;

/******************************************************************************/
// Flight Control Functions
/******************************************************************************/
void nav_setup(void)
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

/*
void nav_update(void)
{
    nav_capturePitch();
    nav_captureRoll();
    nav_captureAltitude();
    nav_captureHeading();
}
*/

//Lower Level Utility
/*****************************************************/
void nav_capturePitch(void) 				// updates desired pitch with current pitch
{
    // Update ACCEL
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();
    a_updatePitchDeg();

    // Update Nav
    nav_actualPitch = a_pitch;
    nav_pitchDiff = nav_actualPitch - nav_desiredPitch;
}

void nav_captureRoll(void)  				// updates desired roll with current roll
{
    // Update ACCEL
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();
    a_updateRollDeg();

    // Update Nav
    nav_actualRoll = a_roll;
    nav_rollDiff = nav_actualRoll - nav_desiredRoll;
}

void nav_captureAltitude(void)  				// updates desired roll with current roll
{
    // Update BMP
    b_updateTempC();
    b_updatePressure();
    b_updateAltitude();

    // Update Nav
    nav_actualAltitude = b_altitude;
    nav_altitudeDiff = nav_actualAltitude - nav_desiredAltitude;
}

void nav_captureHeading(void)
{
    // Update ACCEL
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();

    // Update MAG
    m_updateXRaw();
    m_updateYRaw();
    m_updateZRaw();

    // Update Nav
    m_updateTiltHeading(a_pitch, a_roll);
    nav_actualHeading = m_heading;
    nav_headingDiff = nav_actualHeading - nav_desiredHeading;
}

void nav_setPitch(float p)
{
    nav_desiredHeading = p;
}

void nav_setRoll(float r)
{
    nav_desiredRoll = r;
}

void nav_setAltitude(float a)
{
    nav_desiredAltitude = a;
}

void nav_setHeading(float h)
{
    nav_desiredHeading = h;
}

