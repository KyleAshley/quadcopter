/*****************************************************************************/
// i2c.c
// Author: Kyle Ashley
/*****************************************************************************/

#pragma once
#include "i2c.h"
#include <hidef.h>          /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <math.h>

#include "LCD_Write.h"
#include "delays.h"
#include "accel.h"

#define IBEN 0x80
#define IBIE 0x40
#define MSSL 0x20
#define TXRX 0x10
#define TXAK 0x08
#define RSTA 0x04
#define IBSWAI 0x01

#define IBB 0x20
#define IBIF 0x02
#define RXAK 0x01

#define OSS 0x00

void openI2C(unsigned char baudrate)
{
  // 100kHz rate
  IBCR = IBEN;                 // Enable I2C module
  IBFD = baudrate;             // set Baudrate
  IBSR &= ~0x02;               // clear IBIF flag
}

unsigned char I2C_Read_Byte(unsigned char slaveAddress, unsigned char internalAddress)
{
  unsigned char readVal;
  
  while(IBSR & IBB);                  // wait for idle
  
  // Send address the slave device
  IBCR = IBEN | TXRX | MSSL;          // take bus
  IBDR = slaveAddress;                // write slave address
  while(!(IBSR & IBIF));              // wait for transmission
  IBSR = IBIF;                        // clear IBIF flag
  while((IBSR & RXAK));
  
  // Send address of read register
  IBDR = internalAddress | 0x80;     // set register address                  
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  IBCR |= RSTA;                       //repeat start
  
  // Send register read address
  IBDR = (slaveAddress | 0x01);      // send slave address to read
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  // Read the value
  IBCR |= TXAK;                      // prepare to NACK 
  IBCR &= ~TXRX;					           // switch to RX
  readVal = IBDR;                    // read dummy value
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  
  // End Transmission
  IBCR &= ~MSSL;                     // send STOP bit
  
  readVal = IBDR;
  return readVal;
}

void I2C_Write_Byte(unsigned char slaveAddress, unsigned char internalAddress, unsigned char data)
{
  while(IBSR & IBB);                 // wait for idle
  
  // Start Transmission
  IBCR = IBEN | TXRX | MSSL;         // take bus
  
  // Send address the slave device
  IBDR = slaveAddress;               // write slave address
  while(!(IBSR & IBIF));             // wait for transmission
  IBSR = IBIF;                       // clear IBIF flag
  while((IBSR & RXAK));
  
   // Send address of write register
  IBDR = internalAddress | 0x80;     // set register address                  
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  // Send write value
  IBDR = data;      		             // send data to write
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  // End Transmission
  IBCR &= ~MSSL;                     // send STOP bit
}

long I2C_Read_2Bytes(unsigned char slaveAddress, unsigned char internalAddress)
{
  long readVal;
  unsigned char xH, xL;
  unsigned char temp;
  
  while(IBSR & IBB);                  // wait for idle
  
  // Send address the slave device
  IBCR = IBEN | TXRX | MSSL;          // take bus
  IBDR = slaveAddress;                // write slave address
  while(!(IBSR & IBIF));              // wait for transmission
  IBSR = IBIF;                        // clear IBIF flag
  while((IBSR & RXAK));
  
  // Send address of read register
  IBDR = internalAddress;    // set register address                  
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  IBCR |= RSTA;                       //repeat start
  
  // Send register read address
  IBDR = (slaveAddress | 0x01);      // send slave address to read
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  IBCR &= ~(TXAK | TXRX);                     // prepare to ACK and RX
  temp = IBDR;                                // read dummy value / trigger read
  while(!(IBSR & IBIF));                      // wait for transfer
  IBSR = IBIF;
  
  IBCR |= TXAK;                                
  xH = IBDR;                                  // Read the value                                                                                          
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  
  // End Transmission
  IBCR &= ~MSSL;                              // send STOP bit
  xL = IBDR;
  
  readVal = (((unsigned long)xH << 8) | xL);
  return (long)readVal;
}


int I2C_Read_3Bytes(unsigned char slaveAddress, unsigned char internalAddress)
{
  int readVal;
  unsigned char xH, xM, xL, temp;
  
  while(IBSR & IBB);                  // wait for idle
  
  // Send address the slave device
  IBCR = IBEN | TXRX | MSSL;          // take bus
  IBDR = slaveAddress;                // write slave address
  while(!(IBSR & IBIF));              // wait for transmission
  IBSR = IBIF;                        // clear IBIF flag
  while((IBSR & RXAK));
  
  // Send address of read register
  IBDR = internalAddress;     // set register address                  
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  IBCR |= RSTA;                       //repeat start
  
  // Send register read address
  IBDR = (slaveAddress | 0x01);      // send slave address to read
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  IBCR &= ~(TXAK | TXRX);             // prepare to ACK and RX
  temp = IBDR;                        // read dummy value
  while(!(IBSR & IBIF));
  IBSR = IBIF;
     
  xH = IBDR;                          // read dummy value
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  
  IBCR |= TXAK;                      // prepare to NACK 
  xM = IBDR;                        // read dummy value
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  
  // End Transmission
  IBCR &= ~MSSL;                     // send STOP bit
  xL = IBDR;
  
  readVal = (((unsigned long)xH << 16) | ((unsigned long)xM << 8) | ((unsigned long)xL >> (8 - OSS)));
  return readVal;
}

void I2C_Read_Bytes(unsigned char slaveAddress, unsigned char internalAddress, int num, int* buffer)
{
  unsigned char temp;
  int i;
  
  while(IBSR & IBB);                  // wait for idle
  
  // Send address the slave device
  IBCR = IBEN | TXRX | MSSL;          // take bus
  IBDR = slaveAddress;                // write slave address
  while(!(IBSR & IBIF));              // wait for transmission
  IBSR = IBIF;                        // clear IBIF flag
  while((IBSR & RXAK));
  
  // Send address of read register
  IBDR = internalAddress;     // set register address                  
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  IBCR |= RSTA;                       //repeat start
  
  // Send register read address
  IBDR = (slaveAddress | 0x01);      // send slave address to read
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  while((IBSR & RXAK));
  
  IBCR &= ~(TXAK | TXRX);             // prepare to ACK and RX
  temp = IBDR;                        // read dummy value
  
  for(i = 0; i < num - 2; i++)
  {
    while(!(IBSR & IBIF));
    IBSR = IBIF;
    buffer[i] = IBDR;                          // read dummy value

  }
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  
  IBCR |= TXAK;                      // prepare to NACK 
  buffer[num-2] = IBDR;                        // read dummy value
  while(!(IBSR & IBIF));
  IBSR = IBIF;
  
  // End Transmission
  IBCR &= ~MSSL;                     // send STOP bit
  buffer[num-1] = IBDR;
}
