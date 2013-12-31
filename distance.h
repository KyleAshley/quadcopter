/***************************************************/
// distance.h
// Author: Kyle Ashley
// Description: For use with the long range IR sharp 
// distance sensor and HCS12
/***************************************************/

#define VRH 5
#define VRL 0

void setupDISTSENSE(void);
double DtoVIN(double, int);
double convertInches(double);
double convertFeet(double);
double convertCM(double);
double getATDCH0(void);
double constrainDist(double);
