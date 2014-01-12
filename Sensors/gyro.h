/***************************************************/
// gyro.h
// Author: Kyle Ashley
// Description: for use with L2g4200d Digital Gyroscope
// with HCS12 microproccessor
/**************************************************/

// GYRO addresses
// GYRO ID
#define g_ID_SA0_w 0xD0
#define g_IDl_SA0_r 0xD1
#define g_ID_SA1_w 0xD2
#define g_ID_SA1_r 0xD3
#define g_ID 0xD2
// 69

// CTRL REGS
#define g_OUT_TEMP 0x26
#define g_CTRL_REG1 0x20
#define g_CTRL_REG2 0x21
#define g_CTRL_REG3 0x22
#define g_CTRL_REG4 0x23
#define g_CTRL_REG5 0x24
#define g_STATUS_REG 0x27
#define g_FIFO_CTRL_REG 0x2E
#define g_FIFO_SRC_REG 0x2F
#define g_INT1_CFG 0x30
#define g_INT1_SRC 0x31

// INTERRUPT STUFF
#define g_INT1_TSH_XH 0x32  
#define g_INT1_TSH_XL 0x33 
#define g_INT1_TSH_YH 0x34 
#define g_INT1_TSH_YL 0x35 
#define g_INT1_TSH_ZH 0x36 
#define g_INT1_TSH_ZL 0x37 
#define g_INT1_DURATION 0x38 

// RESULT REGS
#define g_xH 0x29
#define g_xL 0x28
#define g_yH 0x2B
#define g_yL 0x2A
#define g_zH 0x2D
#define g_zL 0x2C

// GYRO FUNCTION DECL
unsigned int getGyroX(void);
unsigned int getGyroY(void);
unsigned int getGyroZ(void);
void setupGyro(int);
void output_gyroXYZ(unsigned int, unsigned int, unsigned int);
