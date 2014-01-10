/***************************************************************/
// ATD Functions
// Author: Kyle Ashley
// HC9S12 uP
/***************************************************************/
#pragma once
#ifndef ATD_H_
#define ATD_H_

void atd0_powerOn();                // Powers on ATD module 0
void atd0_powerOff();                // Powers off ATD module 0
void atd0_setFFC(int);              // set fast flag clearing on/off
void atd0_setLength(int);           // sets conversion sequence length of ATD module 0
void atd0_setFifo(int);             // turns FIFO mode on or off (1/0)
void atd0_setResolution(int);       // sets ATD module 0 resolution to 8/10bit
void atd0_setJustification(char);   // 'L' for left, 'R for right
void atd0_setScan(int);             // sets ATD Module 0 SCAN bit on/off (1/0)
void atd0_setMulti(int);            // sets ATD Module 0 MULT bit on/off (1/0)
void atd0_setStart(int);            // sets starting channel of ATD conversion sequence (call Last)
float atd0_readChX(int);            // reads data registers of corresponding ATD Data Reg 

#endif // ATD_H_
