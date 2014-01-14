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
#include "lcd.h"
#include "i2c.h"
#include "delays.h"


#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.8
#define M_PI 3.14159265358
#define TCHAR 2000

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
   lcd_COMWRTFIRST(0x33);   //reset sequence provided by data sheet
   lcd_COMWRT(0x32);       //reset sequence provided by data sheet
   lcd_COMWRT(0x28);       //Function set to four bit data length ,2 line, 5 x 7 dot format
   lcd_COMWRT(0x06);       //entry mode set, increment, no shift
   lcd_COMWRT(0x0E);       //Display set, disp on, cursor on, blink off
   lcd_COMWRT(0x01);       //Clear display
   lcd_COMWRT(0x80);       //set start posistion, home position
   delay_LCD_DATA(TCHAR); // wait for LCD setup

  //********************************************************//
  // I2C Stuff
  //********************************************************//
    openI2C(BR);   
    a_setup();

	for(;;)
	{
      a_updateXRaw();
      a_updateYRaw();
      a_updateZRaw();

      // PITCH / ROLL Calculations
      a_updatePitchDeg();
      a_updateRollDeg();

      lcd_outputChar('P');
      lcd_outputDouble(a_pitch);

      lcd_newLine();

      lcd_outputChar('R');
      lcd_outputDouble(a_roll);

      delay_LCD_DATA(TCHAR * 5);
      lcd_clear();
  }
while(1);
}
