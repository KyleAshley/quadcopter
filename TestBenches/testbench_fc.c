/**********************************************/
// TEST BENCH
// AUTHOR: Kyle Ashley
/**********************************************/

#pragma once
#include <hidef.h>      /* common defines and macros */
#include <stdio.h>
#include <math.h>
#include "flightControl.h"
#include "derivative.h"      /* derivative-specific definitions */
#include "navigation.h"
#include "lcd.h"
#include "i2c.h"
#include "delays.h"


#define PITCH     0
#define ROLL      1
#define ALTITUDE  2
#define HEADING   3

#define BR 0x1F                 // 100kHz BR
#define ALPHA 0.8
#define M_PI 3.14159265358

void main(void) {
  /* put your own code here */
  
  // Contains motor correction relationships
  int** pitchOperands;
  int** rollOperands;
  int** altitudeOperands;
  int** headingOperands;

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
    fc_initialize_motors();
    pitchOperands = fc_get_motors(PITCH);
    rollOperands = fc_get_motors(ROLL);
    altitudeOperands = fc_get_motors(ALTITUDE);
    headingOperands = fc_get_motors(HEADING);

    nav_setup();
    nav_setPitch(0.0);
    nav_setRoll(0.0);
    nav_setAltitude(30.0);
    nav_setHeading(0.0);

	for(;;)
	{
        nav_capturePitch();
        fc_correct(PITCH, pitchOperands);
        nav_captureRoll();
        fc_correct(ROLL, rollOperands);
        nav_captureAltitude();
        fc_correct(ALTITUDE, altitudeOperands);
        nav_captureHeading();
        fc_correct(HEADING, headingOperands);

    }
while(1);
}
