/**********************************************/
// GYRO FUNCTIONS
// AUTHOR: Kyle Ashley
// Description: for use with GY-80 and HCS12
/**********************************************/
#pragma once
#include "I2C.h"
#include "gyro.h"
#include "LCD_write.h"
#include "delays.h"
#include <stdio.h>

// initializes the gyro modules
void setupGyro(int scale)
{
  // enable x, y, z, power-on
  I2C_Write_Byte(g_ID, g_CTRL_REG1, 0x0F);
  
  // HPF stuff
  I2C_Write_Byte(g_ID, g_CTRL_REG2, 0x00);
  
  // data read interrupt on INT 2
  I2C_Write_Byte(g_ID, g_CTRL_REG3, 0x08);  //08
  
  // full-scale range control
  if(scale == 250)
  {
    I2C_Write_Byte(g_ID, g_CTRL_REG4, 0x00);
  }
  else if(scale == 500)
  {
    I2C_Write_Byte(g_ID, g_CTRL_REG4, 0x10);  
  }
  else
  {
    I2C_Write_Byte(g_ID, g_CTRL_REG4, 0x30);
  }
  
  // HPF output stuff
  I2C_Write_Byte(g_ID, g_CTRL_REG5, 0x00);
}

// gets raw x-gyro data
unsigned int getGyroX()
{
     unsigned int x;
     unsigned char xH, xL;
     xH = I2C_Read_Byte(g_ID, g_xH);
     xL = I2C_Read_Byte(g_ID, g_xL);
     x = ((xH << 8) | xL);
     return x;
}

// gets raw y-gyro data
unsigned int getGyroY()
{
     unsigned int y;
     unsigned char yH, yL;
     yH = I2C_Read_Byte(g_ID, g_yH);
     yL = I2C_Read_Byte(g_ID, g_yL);
     y = ((yH << 8) | yL);
     return y;
}

// gets raw z-gyro data
unsigned int getGyroZ()
{
     unsigned int z;
     unsigned char zH, zL;
     zH = I2C_Read_Byte(g_ID, g_zH);
     zL = I2C_Read_Byte(g_ID, g_zL);
     z = ((zH << 8) | zL);
     return z;
}

// prints three raw gyro values to the LCD
void output_gyroXYZ(unsigned int gx, unsigned int gy, unsigned int gz)
{
      unsigned char strx[5];
      unsigned char stry[5];
      unsigned char strz[5];
      unsigned int cx, cy, cz;
      unsigned int ctr;
      
      cx = 0;
      cy = 0;
      cz = 0;

      sprintf(strx,"%d",gx);
      while (strx[cx] != '\0') 
  	  {    
        DATWRTFIRST(strx[cx]);
        LCDDelayDATA(DAT);
        cx++;
      }
      
      DATWRTFIRST(' ');  
      sprintf(stry,"%d",gy);
      while (stry[cy] != '\0') 
    	{    
        DATWRTFIRST(stry[cy]);
        LCDDelayDATA(DAT);
        cy++;
      }
      COMWRTFIRST(0xC0);
        
      sprintf(strz,"%d",gz);
      while (strz[cz] != '\0') 
    	{    
        DATWRTFIRST(strz[cz]);
        LCDDelayDATA(DAT);
        cz++;
      }
        
      for(ctr = 0; ctr < 5; ctr++)
      { 
        LCDDelayDATA(TCHAR * 8);
      }
      COMWRTFIRST(0x01);
      LCDDelayDATA(TCHAR); 
}
