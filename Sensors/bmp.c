/**********************************************/
// Barometric Pressure Sensor Functions
// AUTHOR: Kyle Ashley
/**********************************************/
#pragma once
#include <math.h>
#include "bmp.h"
#include "delays.h"
#include "i2c.h"
#include "lcd.h"
#include "derivative.h"      /* derivative-specific definitions */

// Device ID
#define b_ID 0xEE

// Calibration Registers
#define b_AC1 0xAA
#define b_AC2 0xAC
#define b_AC3 0xAE
#define b_AC4 0xB0
#define b_AC5 0xB2
#define b_AC6 0xB4
#define b_B1 0xB6
#define b_B2 0xB8
#define b_MB 0xBA
#define b_MC 0xBC
#define b_MD 0xBE
#define OSS 0

// Control Modes
#define TEMP_MODE 0x2E
#define PRESSURE_MODE_0 0x34
#define PRESSURE_MODE_1 0x74
#define PRESSURE_MODE_2 0xB4
#define PRESSURE_MODE_3 0xF4

// Pressure at Sea Level (Pa)
#define P_AT_SEALEVEL 102765.0738

short b_ac1;
short b_ac2;
short b_ac3;
unsigned short b_ac4;
unsigned short b_ac5;
unsigned short b_ac6;
short b_b1;
short b_b2;
short b_mb;
short b_mc;
short b_md;
short b_b5;
long b_x1, b_x2, b_x3, b_b3, b_b6;
unsigned long b_b4, b_b7;

// Raw Pressure and Raw Temperature Variables
long b_uP;
long b_uT;

// Converted pressure, temperature and alitude values
long b_pressure;
float b_tempC;
float b_altitude;

/******************************************************************************/
// PRE: NONE
// POST: Calibration variables contain values read from device ROM
/******************************************************************************/
void b_setup(void)
{
    int buffer[2];

    // Request Calibration ROM reads
    I2C_Read_Bytes(b_ID, b_AC1, 2, buffer);
    b_ac1 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_AC2, 2, buffer);
    b_ac2 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_AC3, 2, buffer);
    b_ac3 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_AC4, 2, buffer);
    b_ac4 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_AC5, 2, buffer);
    b_ac5 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_AC6, 2, buffer);
    b_ac6 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_B1, 2, buffer);
    b_b1 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_B2, 2, buffer);
    b_b2 = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_MB, 2, buffer);
    b_mb = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_MC, 2, buffer);
    b_mc = (((int)buffer[0] << 8) + buffer[1]);
    I2C_Read_Bytes(b_ID, b_MD, 2, buffer);
    b_md = (((int)buffer[0] << 8) + buffer[1]);
}

/* Control Functions */
/******************************************************************************/
// PRE: NONE
// POST: Sets Control Mode of bmp to read requested values (temp/pressure)
/******************************************************************************/
void b_setControlMode(unsigned int mode)
{
    I2C_Write_Byte(b_ID, 0xF4, mode);
    delay_LCD_DATA(2200);
}

/******************************************************************************/
// PRE: NONE
// POST: Reads raw temperature value from bmp
// VAL TO UPDATE: b_uT
/******************************************************************************/
void b_updateRawTemperature(void)
{
    int buffer[2];
    I2C_Read_Bytes(b_ID, 0xF6, 2, buffer);
    b_uT = (((long)buffer[0] << 8 | buffer[1]));
}

/******************************************************************************/
// PRE: NONE
// POST: Reads raw pressure value from bmp
// VAL TO UPDATE: b_uP
/******************************************************************************/
void b_updateRawPressure(void)
{
    int buffer[3];
    I2C_Read_Bytes(b_ID, 0xF6, 3, buffer);
    b_uP = (long)((((unsigned long) buffer[0] << 16) | ((unsigned long) buffer[1] << 8) | ((unsigned long) buffer[2])) >> (8-OSS));
}

/******************************************************************************/
// PRE: NONE
// POST: Reads raw temperature values and calculates true temperature (C)
// VAL TO UPDATE: b_tempC
/******************************************************************************/
void b_updateTempC(void)
{
    // Request Raw Temperature Reading
    b_updateRawTemperature();

    // Calculate X1, X2, B5
    b_x1 = (((b_uT - (long)b_ac6) * (long)b_ac5) >> 15);
    b_x2 = ((long)b_mc << 11) / (b_x1 + b_md);
    b_b5 = (long)b_x1 + b_x2;

    // Calculate Converted Temperature (C)
    b_tempC = (float)(((b_b5 + 8) >> 4) / 10.0);
}

/******************************************************************************/
// PRE: b_updateTempC has been called and tempC contains updated value
// POST: Reads raw pressure value from bmp and calculates true pressure (Pa)
// VAL TO UPDATE: b_pressure
/******************************************************************************/
void b_updatePressure(void)
{
    // Request Raw Pressure Reading
    b_updateRawPressure();

    //Calculate B6
  	b_b6 = b_b5 - 4000;

    // Calculate B3
    b_x1 = ((long)b_b2 * ((b_b6 * b_b6)>>12))>>11;
    b_x2 = ((long)b_ac2 * b_b6)>>11;
    b_x3 = b_x1 + b_x2;
    b_b3 = ((((long)b_ac1*4 + b_x3)<<OSS) + 2)>>2;

    // Calculate B4
    b_x1 = ((long)b_ac3 * b_b6)>>13;
    b_x2 = ((long)b_b1 * ((b_b6 * b_b6)>>12))>>16;
    b_x3 = ((b_x1 + b_x2) + 2)>>2;
    b_b4 = ((unsigned long)b_ac4 * (unsigned long)(b_x3 + 32768))>>15;

    // Calculate Pressure
    b_b7 = ((unsigned long)b_uP - b_b3) * (unsigned long)(50000>>OSS);
    if (b_b7 < 0x80000000)
      b_pressure = (b_b7<<1)/b_b4;
    else
      b_pressure = (b_b7/b_b4)<<1;
    b_x1 = (b_pressure>>8) * (b_pressure>>8);
    b_x1 = (b_x1 * 3038)>>16;
    b_x2 = (-7357 * b_pressure)>>16;
    b_pressure += ((b_x1 + b_x2 + (long)3791)>>4);
}

/******************************************************************************/
// PRE: b_updateTempC and b_updatePressure have been called and respective
//      readings have been updated
// POST: Calculates true altitude relative to relative pressure at Sea Level
// VAL TO UPDATE: b_altitude
/******************************************************************************/
void b_updateAltitude(void)
{
    float A, B, C;

    // Calculate Altitude after receiving raw pressure reading
    A = (float)b_pressure/P_AT_SEALEVEL;
    B = (float)1/5.25588;
    C = (float)pow(A,B);
    C = (float)1 - C;
    C = (float)C /0.0000225577;

    // Assign converted altiutude value
    b_altitude = C;
}
