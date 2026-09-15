#include "Definitions.h"
#include "Movement.h"

volatile unsigned long leftTicks = 0;
volatile unsigned long rightTicks = 0;

void turnRight(int speedVal) {
  float arcCM = (3.14159 * WHEELBASE_CM) / 4.0;
  unsigned long targetTicks = (unsigned long)(arcCM * COUNTS_PER_CM);
  leftTicks = 0;
  rightTicks = 0;

  analogWrite(IN1, speedVal);  analogWrite(IN2, 0);
  analogWrite(IN3, 0);         analogWrite(IN4, speedVal);

  while ((leftTicks + rightTicks) / 2 < targetTicks) {
  }

  stopMotors();
  delay(300);
}

void turnLeft(int speedVal) {
  float arcCM = (3.14159 * WHEELBASE_CM) / 4.0;
  unsigned long targetTicks = (unsigned long)(arcCM * COUNTS_PER_CM);
  leftTicks = 0;
  rightTicks = 0;

  analogWrite(IN1, 0);         analogWrite(IN2, speedVal);
  analogWrite(IN3, speedVal);  analogWrite(IN4, 0);

  while ((leftTicks + rightTicks) / 2 < targetTicks) {
  }
  moveDistance(200);
  stopMotors();
  delay(300);
}

void ReturnBack(int speedVal) {
  float arcCM = (3.14159 * WHEELBASE_CM) / 2.0;
  unsigned long targetTicks = (unsigned long)(arcCM * COUNTS_PER_CM);
  leftTicks = 0;
  rightTicks = 0;

  analogWrite(IN1, speedVal);  analogWrite(IN2, 0);
  analogWrite(IN3, 0);         analogWrite(IN4, speedVal);

  while ((leftTicks + rightTicks) / 2 < targetTicks) {
  }
  moveDistance(200);
  stopMotors();
  delay(300); 
}

void stopMotors() {
  analogWrite(IN1, 0);  analogWrite(IN2, 0);
  analogWrite(IN3, 0);  analogWrite(IN4, 0);
}

void leftISR() {
  //leftTicks++;
  if (digitalRead(ENC_L_B ) == HIGH) {
    leftTicks++;
  } else {
    leftTicks--;
  }
}

void rightISR() {
  //rightTicks++;
  if (digitalRead(ENC_R_B ) == HIGH) {
    leftTicks++;
  } else {
    leftTicks--;
  }
}

void leftISR1() {
  leftTicks++;
}

void rightISR1() {
  rightTicks++;
}

void moveDistance(int speedVal) {
  unsigned long targetTicks = (unsigned long)(16 * COUNTS_PER_CM);
  leftTicks = 0;
  rightTicks = 0;

  analogWrite(IN1, speedVal);  analogWrite(IN2, 0);
  analogWrite(IN3, speedVal);  analogWrite(IN4, 0);

  while ((leftTicks + rightTicks) / 2 < targetTicks) {
  }
  stopMotors();
  delay(300); // مهلة استقرار للقصور الذاتي
}