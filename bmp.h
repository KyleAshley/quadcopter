/******************************************************/
// BMP FUNCTIONS
// Author: Kyle Ashley
// Description: For use with bmp085 barometric pressure
// sensor with HCS12 microcontroller
/******************************************************/

#define b_ID 0xEE

// calibration regs
#define b_AC1 0xAA
#define b_AC2 0xAC
#define b_AC3 0xAE
#define b_AC4 0xB0
#define b_AC5 0xB2
#define b_AC6 0xB4
#define b_B1 0xB6
#define b_B2 0xB8
#define b_MB 0xBA
#define b_MC 0xBC
#define b_MD 0xBE
#define OSS 0

#define TEMP_MODE 0x2E
#define PRESSURE_MODE_0 0x34
#define PRESSURE_MODE_1 0x74
#define PRESSURE_MODE_2 0xB4
#define PRESSURE_MODE_3 0xF4

extern short ac1, ac2, ac3;
extern unsigned short ac4, ac5, ac6;
extern short b1, b2, mb, mc, md;
extern short b5;
extern long uP;
extern long uT;
extern long x1, x2, x3, b3, b6;
extern unsigned long b4, b7;
extern long p;
extern long x1, x2;
extern double temp;

void calibrateBMP(void);
void setBMPControlMode(unsigned int);

double getBMPTempC(void);
long getBMPPressure(void);

long getBMPRawPressure(void);
long getBMPRawTemperature(void);
float calcAltitude(long);
