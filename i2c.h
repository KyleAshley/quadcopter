/***************************************************/
// I2C Functions
// Author: Kyle Ashley
/***************************************************/

void openI2C(unsigned char);
unsigned char I2C_Read_Byte(unsigned char, unsigned char);
void I2C_Write_Byte(unsigned char, unsigned char, unsigned char);
long I2C_Read_2Bytes(unsigned char, unsigned char);
int I2C_Read_3Bytes(unsigned char, unsigned char);
void I2C_Read_Bytes(unsigned char, unsigned char, int, int*);
