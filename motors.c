#pragma once
#include "motors.h"
#include "derivative.h"
#include "delays.h"

#define MINTHROTTLE 6000       // corresponds to 1ms PW (6000)
#define MAXTHROTTLE 12000      // corresponds to 2ms PW (12000)

int throttle = 0;
void initializeMotors()
{
    PWMCTL = 0xFC;    // all 16 bit channels
    PWMCLK = 0xFF;    // all scaled clocks
    PWMPOL = 0xFF;	   
    PWMSCLA = 1;
    PWMSCLB = 1;
    PWMPRCLK = 0x11;   // PRE = 2
     
    // channel 1
    PWMPER0 = 0x4E;    // 20000
    PWMPER1 = 0x20; 
    PWMDTY0 = 0x17;    // 6000
    PWMDTY1 = 0x70;
     
    // channel 3
    PWMPER2 = 0x4E;    // 20000
    PWMPER3 = 0x20; 
    PWMDTY2 = 0x17;    // 6000
    PWMDTY3 = 0x70;
	   
    // channel 5
    PWMPER4 = 0x4E;    // 20000
    PWMPER5 = 0x20; 
    PWMDTY4 = 0x17;    // 6000
    PWMDTY5 = 0x70;
	   
    // channel 7
    PWMPER6 = 0x4E;    // 20000
    PWMPER7 = 0x20;  
    PWMDTY6 = 0x17;    // 6000
    PWMDTY7 = 0x70;
     
    PWME |= 0xFF;      // enable all channels
    LCDDelaySCROLL(270);  
	   
}

void thrustPOS(int motor, int percent)
{
  int val;
  val = percent * 200;
  
  if(motor == 1)
  {
    if((PWMDTY0 << 8) + PWMDTY0 < MAXTHROTTLE)
    {
     PWMDTY0 += val / 512; 
     PWMDTY1 += val % 512; 
    }
  }
  
  if(motor == 2)
  {
    if((PWMDTY0 << 8) + PWMDTY0 < MAXTHROTTLE)
    {
     PWMDTY2 += val / 512; 
     PWMDTY3 += val % 512; 
    }
  }
  
    if(motor == 3)
  {
    if((PWMDTY0 << 8) + PWMDTY0 < MAXTHROTTLE)
    {
     PWMDTY4 += val / 512; 
     PWMDTY5 += val % 512; 
    }
  }
  
    if(motor == 4)
  {
    if((PWMDTY0 << 8) + PWMDTY0 < MAXTHROTTLE)
    {
     PWMDTY6 += val / 512; 
     PWMDTY7 += val % 512; 
    }
  }
}

void thrustNEG(int motor, int percent)
{
  int val;
  val = percent * 200;
  
  if(motor == 1)
  {
    if((PWMDTY0 << 8) + PWMDTY0 > MINTHROTTLE)
    {
     PWMDTY0 -= val / 512; 
     PWMDTY1 -= val % 512; 
    }
  }
  
  if(motor == 2)
  {
    if((PWMDTY0 << 8) + PWMDTY0 > MINTHROTTLE)
    {
     PWMDTY2 -= val / 512; 
     PWMDTY3 -= val % 512; 
    }
  }
  
    if(motor == 3)
  {
    if((PWMDTY0 << 8) + PWMDTY0 > MINTHROTTLE)
    {
     PWMDTY4 -= val / 512; 
     PWMDTY5 -= val % 512; 
    }
  }
  
    if(motor == 4)
  {
    if((PWMDTY0 << 8) + PWMDTY0 > MINTHROTTLE)
    {
     PWMDTY6 -= val / 512; 
     PWMDTY7 -= val % 512; 
    }
  }
}
    
