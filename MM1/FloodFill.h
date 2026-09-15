//MAHER
//ENCODERS MEMORY 

#pragma once
#include <Arduino.h>

// أبعاد المتاهة القياسية
#define MAZE_SIZE 16

// مصفوفات الذاكرة الداخلية المشتركة للروبوت (الجدران والمسافات)
extern int dist[MAZE_SIZE][MAZE_SIZE];
extern bool memTopWall[MAZE_SIZE][MAZE_SIZE];
extern bool memBottomWall[MAZE_SIZE][MAZE_SIZE];
extern bool memLeftWall[MAZE_SIZE][MAZE_SIZE];
extern bool memRightWall[MAZE_SIZE][MAZE_SIZE];

// دالة تهيئة مصفوفة المسافات الافتراضية باتجاه المركز
void initDistances();

// دالة فحص وصول الروبوت إلى منطقة المركز
bool isAtCenter(int x, int y);

// دالة العثور على أقل مسافة بين الخلايا المجاورة المفتوحة
int getMinOpenNeighbor(int x, int y);

// دالة إضافة الخلايا المجاورة المتاحة إلى طابور المعالجة
void pushOpenNeighbors(int x, int y);

// دالة خوارزمية فلوود فيل الأساسية لإعادة حساب المسافات
void floodFill(int startX, int startY);