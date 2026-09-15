#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>

void initEncoders();
void turnRight(int speedVal);
void turnLeft(int speedVal);
void ReturnBack(int speedVal);
void stopMotors();
void moveDistance( int speedVal);

void leftISR();
void rightISR();
void leftISR1();
void rightISR1();

void MDVSetup();
void EncodersSetup();

#endif