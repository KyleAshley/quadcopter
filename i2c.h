/***************************************************/
// IIC Functions
// AUTHOR: Kyle Ashley
// Description: Enables easy IIC protocol communication
// on HC9S12 uP and asscoiated devices
/***************************************************/
#ifndef I2C_H_
#define I2C_H_

void openI2C(unsigned char);                                        // initializes IIC communication with given slave address
unsigned char I2C_Read_Byte(unsigned char, unsigned char);          // Reads one byte from given slave and internal address
void I2C_Write_Byte(unsigned char, unsigned char, unsigned char);   // Writes one byte to given slave and internal address
void I2C_Read_Bytes(unsigned char, unsigned char, int, int*);       // Places result of multiple IIC reads into given buffer

#endif
