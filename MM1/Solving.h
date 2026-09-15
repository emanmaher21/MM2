#ifndef SOLVING_H
#define SOLVING_H
#include <Arduino.h>

#define MAX_PATH_LENGTH 256

// هيكل بسيط لتمثيل كل نقطة (إحداثي X و Y)
struct PathStep {
  int x;
  int y;
};

// مصفوفة المسار المحسوب والمسجل أثناء الاستكشاف
extern PathStep shortestPath[MAX_PATH_LENGTH];
extern int shortestPathLength;

// مصفوفة تاريخ حركة الروبوت أثناء الاستكشاف
extern PathStep exploreHistory[MAX_PATH_LENGTH];
extern int exploreHistoryLength;

// دالة لإضافة خطوة جديدة في تاريخ الحركة أثناء الاستكشاف
void recordStep(int x, int y);

// دالة حساب أقصر مسار وإزالة الحلقات المغلقة (مطابقة لكود computeShortestPath في Solving.java)
void computeShortestPath();
PathStep* printShortestPath();
// دالة طباعة المسار المحسوب عبر السيريال
void printShortestPath();
#endif