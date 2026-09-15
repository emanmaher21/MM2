//HITHAM
#include "Solving.h"

// مصفوفة لتخزين المسار النهائي المقتطع
PathStep shortestPath[MAX_PATH_LENGTH];
int shortestPathLength = 0;

// مصفوفة تاريخ الحركة المباشر للروبوت
PathStep exploreHistory[MAX_PATH_LENGTH];
int exploreHistoryLength = 0;

//exploringتسجيل كل خطوه يخطوها اثناء ال 
void recordStep(int x, int y) {
  if (exploreHistoryLength < MAX_PATH_LENGTH) {
    exploreHistory[exploreHistoryLength].x = x;
    exploreHistory[exploreHistoryLength].y = y;
    exploreHistoryLength++;
  }
}

/*
 * دالة حساب أقصر مسار (computeShortestPath):
 * مأخوذة ومطابقة لمنطق computeShortestPath في كود Solving.java.
 * تمر على تاريخ الخطوات؛ إذا زار الروبوت نفس الخلية مرتين (Loop)،
 * يتم حذف جميع الحركات الزائدة والرجوع إلى أول زيارة لتلك الخلية.
 */
void computeShortestPath() {
  // تفريغ مصفوفة المسار المختصر القديم
  shortestPathLength = 0;

  for (int stepIdx = 0; stepIdx < exploreHistoryLength; stepIdx++) {
    PathStep step = exploreHistory[stepIdx];
    int existingIndex = -1;

    // البحث عما إذا كانت الخلية الحالية موجودة مسبقاً في المسار
    for (int i = 0; i < shortestPathLength; i++) {
      if (shortestPath[i].x == step.x && shortestPath[i].y == step.y) {
        existingIndex = i;
        break;
      }
    }

    // في حال تكرار الخلية (Loop): حذف كل الخطوات التي تلت الزيارة الأولى
    if (existingIndex >= 0) {
      shortestPathLength = existingIndex + 1;
    } else {
      // إضافة الخطوة الجديدة إذا كانت أول زيارة
      if (shortestPathLength < MAX_PATH_LENGTH) {
        shortestPath[shortestPathLength] = step;
        shortestPathLength++;
      }
    }
  }
}
void printShortestPath() {
  Serial.print(F("Computed Shortest Path Length: "));
  Serial.println(shortestPathLength);

  for (int i = 0; i < shortestPathLength; i++) {
    Serial.print(F("Step "));
    Serial.print(i);
    Serial.print(F(": ("));
    Serial.print(shortestPath[i].x);
    Serial.print(F(", "));
    Serial.print(shortestPath[i].y);
    Serial.println(F(")"));
  }
}