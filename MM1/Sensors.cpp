//MAHER
//TOF & 2 IR DIGITAL & 1 IR ANALOG (MAY BE THEY )
/*#include "Definitions.h"
#include <Wire.h>
#include <VL53L0X.h>



// front
int TOFDIS() {
  uint16_t distance = sensor.readRangeSingleMillimeters();
  return(distance);
  delay(100);
}

//left
bool isLeftWallDetected() {
  return (readIRLeftRaw() > IR_SIDE_WALL_THRESHOLD);
}

//right
bool isRightWallDetected() {
  return (readIRRightRaw() > IR_SIDE_WALL_THRESHOLD);
}

//front
bool isFrontWallDetected() {
  bool tofWall = (readFrontDistance() <= FRONT_WALL_THRESHOLD_MM);
 // bool analogWall = (readIRFrontRaw() > IR_FRONT_WALL_THRESHOLD);
  return (tofWall);
}*/