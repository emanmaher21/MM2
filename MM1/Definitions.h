#ifndef DEFINITIONSREC_H
#define DEFINITIONSREC_H

#include <Wire.h>
#include <Arduino.h>
#include <VL53L0X.h>

extern VL53L0X tofSensor;

//sensors 
#define IR_LEFT_PIN        PA_4    
#define IR_RIGHT_PIN       PA_5 
#define IR_FRONT_PIN       PA_6  
#define IR_SIDE_WALL_THRESHOLD   500
//tof  
#define FRONT_WALL_THRESHOLD_MM   30

// Motors
#define IN1  PA_0
#define IN2  PA_1
#define IN3  PA_2
#define IN4  PA_3

// Encoders
#define ENC_L_A     PB4
#define ENC_L_B     PB5
#define ENC_R_A     PB8
#define ENC_R_B     PB9

// --- Physical Parameters ---
const float COUNTS_PER_REV = 350.0; 
const float WHEEL_DIAMETER_CM = 3.2; 
const float WHEEL_CIRCUMFERENCE_CM = 3.14159 * WHEEL_DIAMETER_CM; // ~10.053 cm
const float COUNTS_PER_CM = COUNTS_PER_REV / WHEEL_CIRCUMFERENCE_CM;

// Distance between left and right wheel centerlines in cm
const float WHEELBASE_CM = 8.0; 

const float CELL_LENGTH_CM = 16.0f; // طول خلية المتاهة القياسي (16 سم)
// Base motor speed (0 - 255)
const int BASE_SPEED = 140;

extern volatile unsigned long leftTicks;
extern volatile unsigned long rightTicks;

#endif