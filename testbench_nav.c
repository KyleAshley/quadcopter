/**********************************************/
// TEST BENCH
// AUTHOR: Kyle Ashley
/**********************************************/

#pragma once
#include <hidef.h>      /* common defines and macros */
#include <stdio.h>
#include <math.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "navigation.h"
#include "lcd.h"
#include "i2c.h"
#include "delays.h"


#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.8
#define M_PI 3.14159265358

void main(void) {
  /* put your own code here */
  int pitch, roll, altitude, heading;

   CLKSEL = 0;
   SYNR = 2;
   REFDV = 0;
   while (!(CRGFLG & 0x08));
   CLKSEL = 0x80;                 // Bus = 24Mhz

   MCCTL = 0x4C;                  // PRE = 16 FLMC = 1

   EnableInterrupts;

   // INITIALIZE LCD


  //********************************************************//
  // I2C Stuff
  //********************************************************//
    nav_setup();
    nav_setPitch(0.0);
    nav_setRoll(0.0);
    nav_setAltitude(30.0);
    nav_setHeading(0.0);

	for(;;)
	{
        nav_update();
        pitch = nav_isHoldingPitch();
        roll = nav_isHoldingRoll();
        altitude = nav_isHoldingAltitude();
        heading = nav_isHoldingHeading();

        if(pitch == 0)
        {
            lcd_DATWRTFIRST('P');
            LCDDelayDATA(TCHAR);
        }
        else
        {
            lcd_DATWRTFIRST(' ');
            LCDDelayDATA(TCHAR);
        }

        if(roll == 0)
        {
            lcd_DATWRTFIRST('R');
            LCDDelayDATA(TCHAR);
        }
        else
        {
            lcd_DATWRTFIRST(' ');
            LCDDelayDATA(TCHAR);
        }


        if(altitude == 0)
        {
            lcd_DATWRTFIRST('A');
            LCDDelayDATA(TCHAR);
        }
        else
        {
            lcd_DATWRTFIRST(' ');
            LCDDelayDATA(TCHAR);
        }

        if(heading == 0)
        {
            lcd_DATWRTFIRST('H');
            LCDDelayDATA(TCHAR);
        }
        else
        {
            lcd_DATWRTFIRST(' ');
            LCDDelayDATA(TCHAR);
        }

      LCDDelayDATA(TCHAR * 7);
      COMWRTFIRST(0x01);
      LCDDelayDATA(TCHAR);
    }
while(1);
}
