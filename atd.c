/*********************************************************************/
// ATD Functions
// Author: Kyle Ashley
// HC9S12 uP
/*********************************************************************/
#include "derivative.h"
#include "atd.h"

void atd0_powerOn()                          // Powers on ATD module 0
{
    ATD0CTL2 |= 0x80; 
    LCDDelayDATA(60);
}

void atd0_powerOff()                          // Powers off ATD module 0
{
    ATD0CTL2 &= 0x00;
}

void atd0_setFFC(int state)              // set fast flag clearing on/off
{
    if(state == 1)
      ATD0CTL2 |= 0x40;
    if(state == 0)
      ATD0CTL2 &= 0xBF;
}

void atd0_setLength(int len)                  // sets conversion sequence length of ATD module 0
{
    unsigned char regVal = (unsigned char)len;
    ATD0CTL3 &= 0x07;                         // clear length bits
    ATD0CTL3 |= (regVal << 3);                 // set appropriate bits
}

void atd0_setFifo(int state)                 // turns FIFO mode on or off (1/0)
{
    ATD0CTL3 &= 0xFB;                       // clear FIFO bit
    ATD0CTL3 |= (state << 2);
}

void atd0_setResolution(int res)            // sets ATD module 0 resolution to 8/10bit
{
    if(res == 10)
        ATD0CTL4 &= 0x7F;
    if(res == 8)
        ATD0CTL4 |= 0x80;
}

void atd0_setJustification(char state)      // 'L' for left, 'R for right
{
    if(state == 'l' || state == 'L')
        ATD0CTL5 &= 0x7F;
    if(state == 'r' || state == 'R')
        ATD0CTL5 |= 0x80;
}

void atd0_setScan(int state)                 // sets ATD Module 0 SCAN bit on/off (1/0)
{
    if(state == 1)
        ATD0CTL5 |= 0x20;
    if(state == 0)
        ATD0CTL5 &= 0xDF;
}

void atd0_setMulti(int state)                // sets ATD Module 0 MULT bit on/off (1/0)
{
    if(state == 1)
        ATD0CTL5 |= 0x10;
    if(state == 0)
        ATD0CTL5 &= 0xEF;
}

void atd0_setStart(int val)                  // sets starting channel of ATD conversion sequence
{
    unsigned char regVal = (unsigned char)val;
    ATD0CTL5 &= 0xF0;
    ATD0CTL5 |= regVal;
}

float atd0_readChX(int ch)              // reads data registers of corresponding ATD channel
{
    unsigned int digH, digL;
    double digi;

    if(ch == 0)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR0H;
        digL = ATD0DR0L;

        digH &= 0x03;
        digi = (digH << 8) + digL;
    }

    else if(ch == 1)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR1H;
        digL = ATD0DR1L;

        digH &= 0x03;
        digi = (digH << 8) | digL;
    }

    else if(ch == 2)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR2H;
        digL = ATD0DR2L;

        digH &= 0x03;
        digi = (digH << 8) | digL;
    }

    else if(ch == 3)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR3H;
        digL = ATD0DR3L;

        digH &= 0x03;
        digi = (digH << 8) | digL;
    }
    
    else if(ch == 4)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR4H;
        digL = ATD0DR4L;

        digH &= 0x03;
        digi = (digH << 8) | digL;
    }

    else if(ch == 5)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR5H;
        digL = ATD0DR5L;

        digH &= 0x03;
        digi = (digH << 8) | digL;
    }

    else if(ch == 6)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR6H;
        digL = ATD0DR6L;

        digH &= 0x03;
        digi = (digH << 8) | digL;
    }

    else if(ch == 7)
    {
        while(!(ATD0STAT0 & 0x80));
        digH = ATD0DR7H;
        digL = ATD0DR7L;

        digH &= 0x03;
        digi = (digH << 8) | digL;
    }
    return digi;
}
