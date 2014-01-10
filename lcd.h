/******************************************************/
// lcd.h
// AUTHOR: Kyle Ashley
// Description: Defines the LCD WRITE functions
/******************************************************/
#pragma once
#ifndef LCD_H_
#define LCD_H_

// Upper Level Functions
/*******************************************/
void lcd_setup(void);                       // initializes the lcd on PTK
void lcd_outputDouble(double);              // Outputs up to 15 difits of a double
void lcd_outputFloat(float);                // Outputs up to 15 digits of a float
void lcd_outputChar(unsigned char);         // Outputs a character to current cursor position
void lcd_outputString(char*);               // Outputs a character string starting at current cursor position
void lcd_newLine(void);                     // Sets cursor to the next line
void lcd_clear(void);                       // clears the LCD

// Lower Level Functions
//(For Manual Optimized Writes)
/******************************************/
void lcd_COMWRTFIRST(unsigned char);       // When going from DAT_WRT to COM_WRT or for first command write
void lcd_COMWRT(unsigned char);            // For Subsequent Command writes
void lcd_DATWRTFIRST(unsigned char);       // For writing the first character to LCD
void lcd_DATWRT(unsigned char);            // For Writing subsequent characters to LCD
void lcd_DATWRTLAST(unsigned char);        // For Writing the last character to LCD

#endif
