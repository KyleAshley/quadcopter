/**********************************************/
// TEST BENCH
// AUTHOR: Kyle Ashley
/**********************************************/

#pragma once
#include <hidef.h>      /* common defines and macros */
#include <stdio.h>
#include <math.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "accel.h"
#include "LCD_write.h"
#include "I2C.h"
#include "delays.h"


#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.8
#define M_PI 3.14159265358

void main(void) {
  /* put your own code here */

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

    a_setup();
    LCDDelayDATA(TCHAR);

	for(;;)
	{
      a_updateXRaw();
      a_updateYRaw();
      a_updateZRaw();

      // PITCH / ROLL Calculations
      a_updatePitchDeg();
      a_updateRollDeg();

      DATWRTFIRST('P');
      outputDouble(a_pitch);

      COMWRTFIRST(0xC0);
      LCDDelayDATA(TCHAR);

      DATWRTFIRST('R');
      outputDouble(a_roll);

      LCDDelayDATA(TCHAR * 8);
      COMWRTFIRST(0x01);
      LCDDelayDATA(TCHAR);
  }
while(1);
}
