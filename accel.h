/********************************************/
// Author: Kyle Ashley
// accel.h
// for HCS12 and ADXL345
/********************************************/
 
#define a_ID 0xA6

#define a_THRESH_TAP 0x1D
#define a_OFSX 0x1E
#define a_OFSY 0x1F
#define a_OFSZ 0x20
#define a_DUR 0x21
#define a_LATENT 0x22
#define a_WINDOW 0x23
#define a_THRESH_ACT 0x24
#define a_THRESH_INACT 0x25
#define a_TIME_INACT 0x26
#define a_ACT_INACT_CTL 0x27
#define a_THRESH_FF 0x28
#define a_TIME_FF 0x29
#define a_TAP_AXES 0x2A
#define a_ACT_TAP_STATUS 0x2B
#define a_BW_RATE 0x2C
#define a_POWER_CTL 0x2D
#define a_INT_ENABLE 0x2E
#define a_INT_MAP 0x2F
#define a_INT_SOURCE 0x30
#define a_DATA_FORMAT 0x31

#define a_DATAX0 0x32
#define a_DATAX1 0x33
#define a_DATAY0 0x34
#define a_DATAY1 0x35
#define a_DATAZ0 0x36
#define a_DATAZ1 0x37

#define a_FIFO_CTL 0x38
#define a_FIFO_STATUS 0x39


void setupAccel(void);
void setAccelRange(int);
double getAccelX(void);
double getAccelY(void);
double getAccelZ(void);
void output_accelXYZ(double, double, double);
double getAccelPitchDeg(double, double);
double getAccelRollDeg(double, double, double);
double getAccelPitchRad(double, double);
double getAccelRollRad(double, double, double);
double lpFilter(double, double, double);
