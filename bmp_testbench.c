/**********************************************/
// TEST BENCH
// AUTHOR: Kyle Ashley
/**********************************************/

#pragma once
#include <hidef.h>      /* common defines and macros */
#include <stdio.h>
#include <math.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "bmp.h"
#include "LCD_write.h"
#include "I2C.h"
#include "delays.h"

#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.5
#define MAGSCALE .92
#define M_PI 3.14159265358

void main(void) {
  /* put your own code here */

 // BMP
  float altSum;
  float avgAlt;
  float old;
  float alt;
  int i;


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
  // I2C Stuff
  //********************************************************//
  openI2C(BR);
  LCDDelayDATA(TCHAR);

  b_setup();
  LCDDelayDATA(TCHAR);

  for(;;)
  {
      b_setControlMode(TEMP_MODE);
      b_updateTempC();

      altSum = 0;
      for(i = 0; i < 5; i++)
      {
        b_setControlMode(PRESSURE_MODE_0);
        b_updatePressure();
        b_updateAltitude();
        //alt = lpFilter(alt, old, 0.5);
        altSum+= b_altitude;
      }

      avgAlt = altSum/5.0;
      //old = avgAlt;

      DATWRTFIRST('T');
      outputDouble((double)b_tempC);

      COMWRTFIRST(0xC0);
      LCDDelayDATA(TCHAR);

      DATWRTFIRST('A');
      outputDouble((double)b_altitude);

      LCDDelayDATA(TCHAR * 6);
      COMWRTFIRST(0x01);
      LCDDelayDATA(TCHAR);

   }
   while(1);
  }

