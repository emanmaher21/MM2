#include "Simulation.h"
#include "Movement.h"
#include "Definitions.h"

int currentSimulationIndex = 0;

// إعادة ضبط المحاكاة لموضع البداية
void resetSimulation() {
  currentSimulationIndex = 0;
  if (shortestPathLength > 0) {
    currentX = shortestPath[0].x;
    currentY = shortestPath[0].y;
  }
}

//التحقق من اكتمال المسار
bool isSimulationFinished() {
  return (shortestPathLength == 0 || currentSimulationIndex >= shortestPathLength - 1);
}

// تدوير الروبوت للاتجاه المطلوب بالنسبة للاتجاه الحالي
static void alignToDirection(Direction targetDir, int speedVal) {
  int diff = (targetDir - currentDir + 4) % 4;

  if (diff == 1) {
    turnRight(speedVal);
  } else if (diff == 2) {
    ReturnBack(speedVal);
  } else if (diff == 3) {
    turnLeft(speedVal);
  }
  // إذا كان diff == 0 لا نحتاج لدوران
  currentDir = targetDir;
}
//تنفيذ خطوة واحدة من المسار مع الحركة الفيزيائية
bool stepSimulation(int speedVal) {
  if (isSimulationFinished()) {
    return false;
  }

  currentSimulationIndex++;
  PathStep nextPos = shortestPath[currentSimulationIndex];

  // حساب التغير في الإحداثيات
  int dx = nextPos.x - currentX;
  int dy = nextPos.y - currentY;

  Direction nextDir = currentDir;

  // تحديد الاتجاه الجديد بناءً على dx و dy
  if (dx == 1) {
    nextDir = DIR_EAST;
  } else if (dx == -1) {
    nextDir = DIR_WEST;
  } else if (dy == 1) {
    nextDir = DIR_SOUTH;
  } else if (dy == -1) {
    nextDir = DIR_NORTH;
  }

  // تدوير الروبوت فيزيائياً
  alignToDirection(nextDir, speedVal);

  // التحرك للأمام خلية واحدة
  moveDistance(speedVal);

  // تحديث الإحداثيات
  currentX = nextPos.x;
  currentY = nextPos.y;

  return true;
}

// تنفيذ كامل المسار الأقصر حتى الوصول للمركز
void runFullSimulation(int speedVal) {
  resetSimulation();

  while (!isSimulationFinished()) {
    stepSimulation(speedVal);
    delay(50); // استقرار بسيط بين كل خلية
  }

  stopMotors();
}