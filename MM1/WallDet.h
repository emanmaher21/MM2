#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

// اتجاهات الروبوت الأربعة مطابقة لكود الجافا
enum Direction {
  DIR_NORTH = 0,
  DIR_EAST  = 1,
  DIR_SOUTH = 2,
  DIR_WEST  = 3
};

// المتغيرات الحركية لموقع الروبوت الحالي
extern int currentX;
extern int currentY;
extern Direction currentDir;

// كائن حساس المسافة ToF
extern VL53L0X tofSensor;

// دوال تهيئة وقراءة الحساسات
void initSensors();
int getTofDistance();
int readIRLeftRaw();
int readIRRightRaw();
int readIRFrontRaw();

bool isLeftWallDetected();
bool isRightWallDetected();
bool isFrontWallDetected();

// دوال الاستكشاف وتحديث الذاكرة
bool senseWalls();
void printMemoryDump();