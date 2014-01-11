#include "navigation.h"
#include "mag.h"
#include "accel.h"
#include "bmp.h"
#include "i2c.h"
#include "lcd.h"
#include "delays.h"
#include "distanceSensor.h"

#define BR 0x1F                 // 100kHz BR
#define TCHAR 8000    

/******************************************************************************/
// Flight Control Variables
/******************************************************************************/
float nav_actualPitch[2];
float nav_desiredPitch;
float nav_pitchDiff;

float nav_actualRoll[2];
float nav_desiredRoll;
float nav_rollDiff;

float nav_actualAltitude[2];
float nav_desiredAltitude;
float nav_altitudeDiff;

float nav_actualHeading[2];
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
    nav_actualPitch[1] = nav_actualPitch[0];
    nav_actualPitch[0] = a_pitch;
    nav_pitchDiff = nav_actualPitch[0] - nav_desiredPitch;
}

void nav_captureRoll(void)  				// updates desired roll with current roll
{
    // Update ACCEL
    a_updateXRaw();
    a_updateYRaw();
    a_updateZRaw();
    a_updateRollDeg();

    // Update Nav
    nav_actualRoll[1] = nav_actualRoll[0];
    nav_actualRoll[0] = a_roll;
    nav_rollDiff = nav_actualRoll[0] - nav_desiredRoll;
}

void nav_captureAltitude(void)  				// updates desired roll with current roll
{
    // Update BMP
    b_updateTempC();
    b_updatePressure();
    b_updateAltitude();

    // Update Nav
    nav_actualAltitude[1] = nav_actualAltitude[0];      
    nav_actualAltitude[0] = b_altitude;
    nav_altitudeDiff = nav_actualAltitude[0] - nav_desiredAltitude;
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
    nav_actualHeading[1] = nav_actualHeading[0];
    nav_actualHeading[0] = m_heading;
    nav_headingDiff = nav_actualHeading[0] - nav_desiredHeading;
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

