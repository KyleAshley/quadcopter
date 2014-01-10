/**********************************************************/
// motors.h
// Author: Kyle Ashley
// Purpose: Defines DC motor functions used in a Quadcopter
// alongside HC9S12 uP
/**********************************************************/
#ifndef MOTORS_H_
#define MOTORS_H_

void initializeMotors();
void thrustPOS(int, int);
void thrustNEG(int, int);

void rotatePOS();
void rotateNEG();

void pitchPOS();
void pitchNEG();

void rollPOS();
void rollNEG();

#endif
