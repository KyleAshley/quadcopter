/****************************************************************************/
// Author: Kyle Ashley
// Description: Delay functions used for HC9S12 uProccessor
/****************************************************************************/
#pragma once
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delays.h"

#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02
#define TAS 1                  // 1's here to reduce # MCFLG checks
#define PWEH 1  
#define TH 1    
#define TCHAR 8000    
#define DAT 26

void LCDDelayTH(unsigned int num)      // ACTUAL =41 ns per 'num + Overhead
{                                     // IDEAL  = 10 ns
 unsigned int i = 0;
 MCCNT = 0x01;                       // LOAD = 1
 MCCTL = 0x4C;                       // PRE = 1
 for (i=0; i<num; i++) {
   while (!(MCFLG & 0x80));
   MCFLG = 0x80;  
 }
}

void LCDDelayPWEH(unsigned int num)    // ACTUAL = 458 ns per 'num' + Overhead
{                                     // IDEAL  = 450 ns
 unsigned int j = 0;
 MCCNT = 0x0A;                       // LOAD = 11
 MCCTL = 0x4C;                       // PRE = 1
 for (j=0; j<num; j++) {
   while (!(MCFLG & 0x80));
   MCFLG = 0x80;  
 }
}

void LCDDelayTAS(unsigned int num)     // ACTUAL = 167 ns per 'num' + Overhead
{                                     // IDEAL  = 140 ns
 unsigned int k = 0;
 MCCNT = 0x04;                       // LOAD = 4 
 MCCTL = 0x4C;                       // PRE = 1
 for (k=0; k<num; k++) {
   while (!(MCFLG & 0x80));
   MCFLG = 0x80;  
 }
}

void LCDuDelay(unsigned int num)    // ACTUAL = (1.33 us per 'num' + Overhead) 27 = 40 us
{                                     // IDEAL  = 40 us
 unsigned int l = 0;
 MCCNT = 0x20;                       // LOAD = 32
 MCCTL = 0x4C;                       // PRE = 1
 for (l=0; l<num; l++) {
   while (!(MCFLG & 0x80));
   MCFLG = 0x80;  
 }
}

void LCDDelayDATA(unsigned int num)    // ACTUAL = (40us per 'num' + Overhead) = 40 us
{                                     // IDEAL  = 40 us
 unsigned int m = 0;
 MCCNT = 0x20;                       // LOAD = 32
 MCCTL = 0x4C;                       // PRE = 1
 for (m=0; m<num; m++) {
   while (!(MCFLG & 0x80));
   MCFLG = 0x80;  
 }
}

void LCDDelaySCROLL(unsigned int num)  // 43 ms per 'num' + Overhead
{
 unsigned int n = 0;
 for (n=0; n<num; n++) {
   MCCNT = 0xFFFE;                     // LOAD = 64k
   MCCTL = 0x4F;                       // PRE = 16
   while (!(MCFLG & 0x80));
   MCFLG = 0x80;
 }
}
