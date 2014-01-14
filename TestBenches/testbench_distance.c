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
#include "distanceSensor.h"
#include "lcd.h"
#include "i2c.h"
#include "delays.h"
#include "atd.h"

#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.5
#define MAGSCALE .92
#define M_PI 3.14159265358
#define TCHAR 2000

void main(void) {
  /* put your own code here */
  unsigned int i;

   CLKSEL = 0;
   SYNR = 2;
   REFDV = 0;
   while (!(CRGFLG & 0x08));
   CLKSEL = 0x80;                 // Bus = 24Mhz

   MCCTL = 0x4C;                  // PRE = 16 FLMC = 1
   
   lcd_setup();

   EnableInterrupts;

   
   openI2C(BR);
   delay_LCD_DATA(TCHAR);

   d_setup();
   delay_LCD_DATA(TCHAR);

   for(;;)
   {
      d_readDistance(0);

      lcd_DATWRTFIRST('d');
      delay_LCD_DATA(TCHAR);
      lcd_DATWRTFIRST(':');
      delay_LCD_DATA(TCHAR);
      lcd_outputFloat(d_distance);

      for(i = 0; i < 2; i++)
        delay_LCD_DATA(TCHAR);

      lcd_clear();
    }
   while(1);
  }


