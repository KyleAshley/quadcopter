/******************************************************/
// BMP FUNCTIONS
// Author: Kyle Ashley
// Description: For use with bmp085 barometric pressure
// sensor with HCS12 microcontroller
/******************************************************/
#ifndef BMP_H_
#define BMP_H_

// Calibration variables
extern short ac1;
extern short ac2;
extern short ac3;
extern unsigned short ac4;
extern unsigned short ac5;
extern unsigned short ac6;
extern short b1;
extern short b2;
extern short mb;
extern short mc;
extern short md;
extern short b5;
extern long x1, x2, x3, b3, b6;
extern unsigned long b4, b7;

// Raw Pressure and Raw Temperature Variables
extern long uP;
extern long uT;

// Converted pressure, temperature and alitude values
extern long b_pressure;
extern float b_tempC;
extern float b_altitude;

// BMP Associated Functions
/******************************************************************************/
void b_setup(void);                     // loads calibration values into variables
void b_setControlMode(unsigned int);    // sets bmp for pressure/temp reading

// MUST CALL THESE THREE IN ORDER
void b_updateTempC(void);               // updates tempC with new sensor reading
void b_updatePressure(void);            // updates pressure with new sensor reading
void b_updateAltitude(void);              // updates altitude with new sensor reading

// Accesories to be called by updates
void b_updateRawPressure(void);
void b_updateRawTemperature(void);

#endif
