                           /**********************************************/
// TEST BENCH
// AUTHOR: Kyle Ashley
/**********************************************/

#pragma once
#include <hidef.h>      /* common defines and macros */
#include <stdio.h>
#include <math.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "gyro.h"
#include "accel.h"
#include "mag.h"
#include "bmp.h"
#include "distanceSensor.h"
#include "LCD_write.h"
#include "I2C.h"
#include "delays.h"
#include "motors.h"

#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.5
#define MAGSCALE .92
#define M_PI 3.14159265358



void main(void) {
  /* put your own code here */
  
  unsigned int gX, gY, gZ; 
  double tcX, tcY;
  double mX, mY, mZ;
  double mSX, mSY, mSZ;
  double aX, aY, aZ;
  double faX = 0;
  double faY = 0;
  double faZ = 0;
  volatile double oldFaX = 0;
  volatile double oldFaY = 0;
  volatile double oldFaZ = 0;
  
  //ACCEL
  double pitchDeg, rollDeg, heading, tiltHeading;
  double pitchRad, rollRad;
  
  //DIST
  unsigned char strDigi[8];
  unsigned char strDist[8];
  unsigned char strVin[8];
  unsigned int i, j, k;
  unsigned char digiH;
  unsigned char digiL;
  double digi, vin, dist, oldDigi;
 
 // BMP
  double temperature;
  unsigned long pressure;
  float altSum;
  float avgAlt; 
  float old;
  float alt;
  
   

   CLKSEL = 0;                      
   SYNR = 2;
   REFDV = 0;
   while (!(CRGFLG & 0x08));
   CLKSEL = 0x80;                 // Bus = 24Mhz
   
   MCCTL = 0x4C;                  // PRE = 16 FLMC = 1
   
   EnableInterrupts;
   
   
   // INITIALIZE LCD
   DDRK = 0xFF;   
   COMWRTFIRST(0x33);   //reset sequence provided by data sheet
   COMWRT(0x32);       //reset sequence provided by data sheet
   COMWRT(0x28);       //Function set to four bit data length ,2 line, 5 x 7 dot format
   COMWRT(0x06);       //entry mode set, increment, no shift
   COMWRT(0x0E);       //Display set, disp on, cursor on, blink off
   COMWRT(0x01);       //Clear display
   COMWRT(0x80);       //set start posistion, home position
   LCDDelayDATA(TCHAR); // wait for LCD setup
   
    
  //********************************************************//
  // MOTOR Stuff
  //********************************************************//
   initializeMotors();
   for(i = 0; i < 10; i++)
   {
     thrustPOS(1, 10);
     outputDouble((PWMDTY0 << 8) + PWMDTY1);
     LCDDelaySCROLL(100);
     COMWRTFIRST(0x01);
     LCDDelayDATA(TCHAR);
   }
   
   for(j = 0; j < 10; j++)
   {
     thrustNEG(1, 10);
     outputDouble((PWMDTY0 << 8) + PWMDTY1);
     LCDDelaySCROLL(100);
     COMWRTFIRST(0x01);
     LCDDelayDATA(TCHAR);
   }
   
   /*
  
   PWMDTY0 = 0x17;    // 1ms
	 PWMDTY1 = 0x70;
	 
	 outputDouble((PWMDTY0 << 8) + PWMDTY1);
   LCDDelaySCROLL(400);
   COMWRTFIRST(0x01);
   LCDDelayDATA(TCHAR);
   
   PWMDTY0 = 0x1F;    // 1.33ms
	 PWMDTY1 = 0x40;
	 
	 outputDouble((PWMDTY0 << 8) + PWMDTY1);
   LCDDelaySCROLL(100);
   COMWRTFIRST(0x01);
   LCDDelayDATA(TCHAR);
      
   PWMDTY0 = 0x27;    // 1.65ms
	 PWMDTY1 = 0x10;
	 
	 outputDouble((PWMDTY0 << 8) + PWMDTY1);
   LCDDelaySCROLL(100);
   COMWRTFIRST(0x01);
   LCDDelayDATA(TCHAR);
   
   PWMDTY0 = 0x2E;    // 2ms
	 PWMDTY1 = 0xE0;
	 
	 outputDouble((PWMDTY0 << 8) + PWMDTY1);
   LCDDelaySCROLL(100);
   COMWRTFIRST(0x01);
   LCDDelayDATA(TCHAR);
   */
  
   while(1);
  }
    