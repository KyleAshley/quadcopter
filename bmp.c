/***************************************************************/
// bmp.c
// Author: Kyle Ashley
// *************************************************************/


#pragma once
#include <math.h>
#include "delays.h"
#include "I2C.h"
#include "bmp.h"
#include "LCD_write.h"
#include "accel.h"

short ac1;
short ac2;
short ac3;
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1;
short b2;
short mb;
short mc;
short md;
short b5;
long uP;
long uT;

long x1, x2, x3, b3, b6;
unsigned long b4, b7;
long p;
long x1, x2;
double temp;

void calibrateBMP(void)
{
  int buffer[2];
  
  I2C_Read_Bytes(b_ID, b_AC1, 2, buffer);
  ac1 = (((int)buffer[0] << 8) + buffer[1]);
  I2C_Read_Bytes(b_ID, b_AC2, 2, buffer);
  ac2 = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_AC3, 2, buffer);
  ac3 = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_AC4, 2, buffer);
  ac4 = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_AC5, 2, buffer);
  ac5 = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_AC6, 2, buffer);
  ac6 = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_B1, 2, buffer);
  b1 = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_B2, 2, buffer);
  b2 = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_MB, 2, buffer);
  mb = (((int)buffer[0] << 8) + buffer[1]); 
  I2C_Read_Bytes(b_ID, b_MC, 2, buffer);
  mc = (((int)buffer[0] << 8) + buffer[1]);
  I2C_Read_Bytes(b_ID, b_MD, 2, buffer);
  md = (((int)buffer[0] << 8) + buffer[1]);      
}

/* Control Functions */
void setBMPControlMode(unsigned int mode) 
{
    I2C_Write_Byte(b_ID, 0xF4, mode);
    LCDDelayDATA(2200);
}		

long getBMPRawTemperature() 
{
    int buffer[2];
    I2C_Read_Bytes(b_ID, 0xF6, 2, buffer);
    return (((long)buffer[0] << 8 | buffer[1]));
}

long getBMPRawPressure()
{
    int buffer[3];
    I2C_Read_Bytes(b_ID, 0xF6, 3, buffer);
    return (long)((((unsigned long) buffer[0] << 16) | ((unsigned long) buffer[1] << 8) | ((unsigned long) buffer[2])) >> (8-OSS));
}
	
double getBMPTempC()
{   
    /*
    long x1, x2;
    long temp;
    */
    uT = getBMPRawTemperature();
    x1 = (((uT - (long)ac6) * (long)ac5) >> 15);
    x2 = ((long)mc << 11) / (x1 + md);
    b5 = (long)x1 + x2;
    temp = (float)(((b5 + 8) >> 4) / 10.0);
    return temp;
}
		
long getBMPPressure()
{   
    uP = getBMPRawPressure();
	
  	b6 = b5 - 4000;
  	
    // Calculate B3
    x1 = ((long)b2 * ((b6 * b6)>>12))>>11;
    x2 = ((long)ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = ((((long)ac1*4 + x3)<<OSS) + 2)>>2;

    // Calculate B4
    x1 = ((long)ac3 * b6)>>13;
    x2 = ((long)b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = ((unsigned long)ac4 * (unsigned long)(x3 + 32768))>>15;

    b7 = ((unsigned long)uP - b3) * (unsigned long)(50000>>OSS);
    if (b7 < 0x80000000)
      p = (b7<<1)/b4;
    else
      p = (b7/b4)<<1;

    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += ((x1 + x2 + (long)3791)>>4);
    return p;
}

float calcAltitude(long pressure)
{
    float A, B, C;
    A = (float)pressure/102265.0738;
    B = (float)1/5.25588;
    C = (float)pow(A,B);
    C = (float)1 - C;
    C = (float)C /0.0000225577;

    return C;
}
