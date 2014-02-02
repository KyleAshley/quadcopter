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
#include "mag.h"
#include "bmp.h"
#include "lcd.h"
#include "i2c.h"
#include "delays.h"

#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.8

void main(void) {
  /* put your own code here */
  float gain = 4.7;

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
    delay_us(TCHAR);

    a_setup();
    delay_us(TCHAR);
    
    m_setup(gain);
    delay_us(TCHAR);

	for(;;)
	{
      m_updateXRaw();
      m_updateYRaw();
      m_updateZRaw();

      a_updateXRaw();
      a_updateYRaw();
      a_updateZRaw();

      // PITCH / ROLL Calculations
      a_updatePitchDeg();
      a_updateRollDeg();

      m_updateTiltHeading(a_pitch, a_roll);

      // Output
      lcd_DATWRTFIRST('H');
      lcd_outputDouble(m_heading);

      lcd_COMWRTFIRST(0xC0);
      delay_LCD_DATA(TCHAR);

      lcd_DATWRTFIRST('R');
      lcd_outputDouble(a_roll);

      delay_LCD_SCROLL(5);
      lcd_COMWRTFIRST(0x01);
      delay_LCD_DATA(TCHAR);
  }
while(1);
}
