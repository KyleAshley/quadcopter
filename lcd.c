/***************************************************/
// LCD WRITE source
// contains timing functions for use in writing to
// LCD, and output accesory functions
/***************************************************/
#pragma once
#include <stdio.h>
#include "lcd.h"
#include "delays.h"
#include "derivative.h"

#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02
#define TAS 1                  // 1's here to reduce # MCFLG checks
#define PWEH 1
#define TH 1
#define TCHAR 8000
#define DAT 26

/*******************************************************/
// PRE: Clock has been set to 24mHz
// POST: LCD is initialized and ready to write to
/*******************************************************/
void lcd_setup(void)
{
   DDRK = 0xFF;
   lcd_COMWRTFIRST(0x33);   //reset sequence provided by data sheet
   lcd_COMWRT(0x32);       //reset sequence provided by data sheet
   lcd_COMWRT(0x28);       //Function set to four bit data length ,2 line, 5 x 7 dot format
   lcd_COMWRT(0x06);       //entry mode set, increment, no shift
   lcd_COMWRT(0x0E);       //Display set, disp on, cursor on, blink off
   lcd_COMWRT(0x01);       //Clear display
   lcd_COMWRT(0x80);       //set start posistion, home position
   LCDDelayDATA(TCHAR); // wait for LCD setup
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: up to 15 digits of a double is output to the LCD
/*******************************************************/
void lcd_outputDouble(double val)
{
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned char string[16];
  sprintf(string,"%8.7f",val);
      while (string[i] != '\0')
  	  {
        lcd_DATWRTFIRST(string[i]);
        LCDDelayDATA(TCHAR);
        i++;
      }
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: up to 15 digits of a float is output to the LCD
/*******************************************************/
void lcd_outputFloat(float val)
{
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned char string[8];
  sprintf(string,"%4.3f",val);
      while (string[i] != '\0')
  	  {
        lcd_DATWRTFIRST(string[i]);
        LCDDelayDATA(TCHAR);
        i++;
      }
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: A single character is output to the LCD
/*******************************************************/
void lcd_outputChar(unsigned char data)         // Outputs a character to current cursor position
{
    lcd_DATWRTFIRST(data);
    LCDDelayDATA(TCHAR);
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: A String of any size is output to the LCD
/*******************************************************/
void lcd_outputString(char* text)              // Outputs a character string starting at current cursor position
{
    int i = 0;

    while(text[i] != '\0')
    {
        lcd_DATWRTFIRST(text[i]);
        LCDDelayDATA(TCHAR);
        i++;
    }
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: Cursor is advanced a single line on LCD
/*******************************************************/
void lcd_newLine()                         // Sets cursor to the next line
{
    lcd_COMWRTFIRST(0xC0);
    LCDDelayDATA(TCHAR);
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: Contents of LCD are cleared
/*******************************************************/
void lcd_clear()
{
  lcd_COMWRTFIRST(0x01);
  LCDDelayDATA(1000);
}


/*********************************************************************************************************************/
// LOW LEVEL ACCESORIES
/*********************************************************************************************************************/

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: Prepares LCD to recieve and writes a command
//       (must be called if switching from data->command)
/*******************************************************/
void lcd_COMWRTFIRST(unsigned char data)    // No LCD DAT delay becuase its the first call to COMWRT
{
       unsigned char x;

       x = (data & 0xF0) >> 2;
       LCD_DATA = LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCD_CTRL = LCD_CTRL & ~RS;
       LCDDelayTAS(TAS);
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
       LCDDelayTH(TH);

       x = (data & 0x0F)<< 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
       LCDDelayTH(TH);
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: Writes a command to the LCD
//       (must be called after switching to command mode)
/*******************************************************/
void lcd_COMWRT(unsigned char data)
{
       unsigned char x;

       x = (data & 0xF0) >> 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCDDelayDATA(DAT);              // wait for LCD to write (40us)
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
       LCDDelayTH(TH);

       x = (data & 0x0F)<< 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
       LCDDelayTH(TH);
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: Prepares and writes data to the LCD
//       (must be called if switching from command->data)
/*******************************************************/
void lcd_DATWRTFIRST(unsigned char data)
{
       unsigned char x;

       x = (data & 0xF0) >> 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCD_CTRL = LCD_CTRL | RS;
       LCDDelayTAS(TAS);
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
       LCDDelayTH(TH);

       x = (data & 0x0F)<< 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
}

/*******************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: Writes data to the LCD
//       (must be called after switching to data mode)
/*******************************************************/
void lcd_DATWRT(unsigned char data)      // TH delay is accounted for in LCD DATA Write Delay
{
       unsigned char x;

       x = (data & 0xF0) >> 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCDDelayDATA(DAT);            // wait for LCD to write (40us)
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
       LCDDelayTH(TH);

       x = (data & 0x0F)<< 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
}

/*************************************************************/
// PRE: Clock has been set to 24mHz, LCD is initialized
// POST: Writes a command to the LCD, optimized for last write
//       (must be called after switching to data mode)
/*************************************************************/
void lcd_DATWRTLAST(unsigned char data)   // NO TH delay because there are no succeeding characters
{
       unsigned char x;

       x = (data & 0xF0) >> 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCDDelayDATA(DAT);             // wait for LCD to write (40us)
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
       LCDDelayTH(TH);

       x = (data & 0x0F)<< 2;
       LCD_DATA =LCD_DATA & ~0x3C;
       LCD_DATA = LCD_DATA | x;
       LCD_CTRL = LCD_CTRL | EN;
       LCDDelayPWEH(PWEH);
       LCD_CTRL = LCD_CTRL & ~EN;
}
