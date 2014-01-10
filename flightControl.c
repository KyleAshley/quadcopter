/********************************************************/
// Flight Control
// AUTHOR: Kyle Ashley
// Data and functions combining sensor output
/********************************************************/
#include "flightControl.h"
#include "mag.h"
#include "accel.h"
#include "bmp.h"
#include "i2c.h"
#include "lcd.h"
#include "delays.h"
#include "distanceSensor.h"

/* Define:
 *     angle_off(ENUM TYPE) - ENUM TYPE 0=PITCH, 1=ROLL, 2=HEADING, 3=ALTITUDE
 *     unsigned int* motorRelationships(ENUM TYPE); // Returns Doubleton (int num_in_array, unsigned int* array)
 *     change_threshold = 
 *
 *
 *
 */

#define PITCH     0
#define ROLL      1
#define HEADING   2
#define ALTITUDE  3

#define POSITIVE   1
#define NEGATIVE  -1

int** get_motors(const int TYPE_CORRECTION)
{
	int **operands = malloc(sizeof(int) + sizeof(int*));
	if (TYPE_CORRECTION == PITCH) {
		*operands[0] = 2;
		operands[1] = malloc((sizeof(unsigned int) + sizeof(int)) * *operands[0]);
		operands[1][0][0] = 0;  // motor
		operands[1][0][1] = POSITIVE; // arbitrary
		operands[1][1][0] = 2;  // opposite motor
		operands[1][1][1] = NEGATIVE; // important that operands[1][0][1] is inverse
	} else if (TYPE_CORRECTION == ROLL) {
		*operands[0] = 2;
		operands[1] = malloc((sizeof(unsigned int) + sizeof(int)) * *operands[0]);
		operands[1][0][0] = 1;  // motor
		operands[1][0][1] = POSITIVE; // arbitrary
		operands[1][1][0] = 3;  // opposite motor
		operands[1][1][1] = NEGATIVE; // important that operands[1][0][1] is inverse
	} else if (TYPE_CORRECTION == HEADING) {
		*operands[0] = 4;
		operands[1] = malloc((sizeof(unsigned int) + sizeof(int)) * *operands[0]);
		operands[1][0][0] = 0;  // motor
		operands[1][0][1] = POSITIVE; // arbitrary
		operands[1][1][0] = 2;  // opposite motor
		operands[1][1][1] = POSITIVE; // important that operands[1][0][1] is inverse
		operands[1][2][0] = 1;  // opposite motor
		operands[1][2][1] = NEGATIVE; // important that operands[1][0][1] is inverse
		operands[1][3][0] = 3;  // opposite motor
		operands[1][3][1] = NEGATIVE; // important that operands[1][0][1] is inverse
	} else if (TYPE_CORRECTION == ALTITUDE) {
		*operands[0] = 4;
		operands[1] = malloc((sizeof(unsigned int) + sizeof(int)) * *operands[0]);
		operands[1][0][0] = 0;  // motor
		operands[1][0][1] = POSITIVE; // arbitrary
		operands[1][1][0] = 2;  // opposite motor
		operands[1][1][1] = POSITIVE; // important that operands[1][0][1] is the same
		operands[1][2][0] = 1;  // motor
		operands[1][2][1] = POSITIVE; // important that operands[1][0][1] is the same
		operands[1][3][0] = 3;  // opposite motor
		operands[1][3][1] = POSITIVE; // important that operands[1][0][1] is inverse
	}
 	return operands;
}

void correct(const int TYPE_CORRECTION, const int** operands)
{
	int i, adjustment_value,
	    theta = angle_off(TYPE_CORRECTION);

	if (satisfactory(theta))
		return;

	for (i = 0;  i < *operands[0]; ++i) {
		if (theta > threshold)
			adjustment_value = get_adjustment(operands[1][i][0]) / 2;
		else if (theta < threshold)
			adjustment_value = get_adjustment(operands[1][i][0]) * 2;
		else
			return;

		adjustment_value = (MAX_VAL/adjustment_value) + get_register_value(operands[1][i][0]);

		adjust(operands[1][i][0], adjustment_value);

	}
}


void adjust(int register_index, int value)
{
	if (operands[1][i][0] == 0) {
		PWMDTY0 += value / 512;
		PWMDTY1 += value % 512;
	} else if (operands[1][i][0] == 1) {
		PWMDTY2 += value/512;
		PWMDTY3 += value % 512;
	} else if (operands[1][i][0] == 2) {
		PWMDTY4 += value/512;
		PWMDTY5 += value % 512;
	} else if (operands[1][i][0] == 3) {
		PWMDTY6 += value/512;
		PWMDTY7 += value % 512;
	}
}
