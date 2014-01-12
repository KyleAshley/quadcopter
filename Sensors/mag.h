/*********************************************************/
// mag.h
// AUTHOR: Kyle Ashley
// Description: For use with HMC5883L magnetometer and
// digital compass alongside HCS12 uP
/*********************************************************/
#ifndef MAG_H_
#define MAG_H_

// MAG Associated Variables
/******************************************************************************/

// Raw Readings
extern float m_xRaw;                    // contain most recent raw magnetometer sensor reads
extern float m_yRaw;
extern float m_zRaw;

extern float m_scale;                   // scalar for raw mag readings
extern float m_heading;                 // calculated tilt-corrected heading (Deg)

// MAG Associated Functions
/******************************************************************************/
void m_setup(float);                    // initilizes mag with given resolution
                                        //(.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1)

void m_setMeasurementMode(unsigned int);   // sets measurement mode (continuous/single shot)

// Raw Sensor Reads
void m_updateXRaw(void);                   // updates the associated raw mag readings
void m_updateYRaw(void);
void m_updateZRaw(void);

// Heading Calculation and Updates
void m_updateTiltHeading(float, float);   // Given pitch and roll degrees, updates tilt corrected heading
/* Accesory */
void m_updateHeading(void);               // updates heading if tilt is past 40Deg


#endif
