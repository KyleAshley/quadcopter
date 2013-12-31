/*********************************************************/
// mag.h
// Author: Kyle Ashley
// Description: For use with HMC5883 magnetometer and HCS12
/*********************************************************/

#define m_ID 0x3C
#define m_CONFIG1 0x00 
#define m_CONFIG2 0x01
#define m_MODE 0x02
#define m_xH 0x03
#define m_xL 0x04
#define m_zH 0x05
#define m_zL 0x06
#define m_yH 0x07
#define m_yL 0x08
#define m_STAT 0x09
#define m_IDA 0x0A
#define m_IDB 0x0B
#define m_IDC 0x0C

void setupMag(float);
void set_m_Scale(float);
void set_m_MeasurementMode(unsigned int);
double getMagX(void);
double getMagY(void);
double getMagZ(void);
double getHeading(double, double);
double getTiltHeading(double, double, double, double, double);

