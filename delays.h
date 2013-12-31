/****************************************************************************/
// Author: Kyle Ashley
// Description: Delay functions used for HC9S12 uProccessor
/****************************************************************************/

void LCDDelayTH(unsigned int num);     // ACTUAL =41 ns per 'num + Overhead
void LCDDelayPWEH(unsigned int num);    // ACTUAL = 458 ns per 'num' + Overhead
void LCDDelayTAS(unsigned int num);     // ACTUAL = 167 ns per 'num' + Overhead
void LCDDelayDATA(unsigned int num);   // ACTUAL = (40us per 'num' + Overhead) = 40 us
void LCDDelaySCROLL(unsigned int num);  // 43 ms per 'num' + Overhead
void LCDuDelay(unsigned int num);    // ACTUAL = (1.33 us per 'num' + Overhead) = 1.33 us
                                                                 
