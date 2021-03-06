/***********************************************/
// Distance Sensor functions
// AUTHOR: Kyle Ashley
// for use with long range Sharp distance sensor
/***********************************************/
#ifndef DISTANCESENSOR_H_
#define DISTANCESENSOR_H_

// ATD Interpretation values
#define VRH 5
#define VRL 0

extern float d_distance;

// Sharp Distance Sensor Associated Functions
/******************************************************************************/
void d_setup(void);

void d_readDistance_20_150(unsigned int);          // reads a distance sensor on the given ATD channel
void d_readDistance_4_30(unsigned int); 

float d_DtoVIN(float, int);               // Converts passed input voltage at given resolution into corresponding digital value
//double d_convertInches(double);
//double d_convertFeet(double);
float d_convertCM_20_150(float);                 // returns corresponding distance in cm
float d_convertCM_4_30(float);
float d_constrain_20_150(float);             // constrains readings to valid range
float d_constrain_4_30(float);
#endif
