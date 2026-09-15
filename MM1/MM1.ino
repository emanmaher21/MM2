#include "Definitions.h"
#include "Movement.h"
#include "FloodFill.h"
#include "WallDet.h"
#include "Solving.h"
#include "Simulation.h"

// حالات عمل الروبوت
enum RobotState {
  STATE_EXPLORING,
  STATE_SOLVING,
  STATE_SPEED_RUN,
  STATE_FINISHED
};

RobotState currentState = STATE_EXPLORING;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("MicroMouse Initializing..."));

  // 1. تهيئة أطراف المحركات
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopMotors();

  // 2. تهيئة الإنكودر
  initEncoders();

  // 3. تهيئة الحساسات (ToF و IR)
  initSensors();

  // 4. تهيئة خريطة المسافات لمتاهة 16x16
  initDistances();

  // تسجيل موضع البداية (0, 0) في سجل المسار
  recordStep(currentX, currentY);

  Serial.println(F("Setup Complete. Starting Exploration in 3 seconds..."));
  delay(3000); // مهلة لوضع الروبوت على أرض المتاهة
}

void loop() {
  switch (currentState) {

    // --- مرحلة الاستكشاف (Flood Fill & Exploration) ---
    case STATE_EXPLORING: {
      // 1. التحقق أولاً من الوصول إلى المركز (الهدف)
      if (isAtCenter(currentX, currentY)) {
        stopMotors();
        Serial.println(F("Center reached! Exploration phase finished."));
        printMemoryDump();
        currentState = STATE_SOLVING;
        break;
      }

      // 2. فحص وتحديث الجدران في الموضع الحالي
      bool newWallDiscovered = senseWalls();

      // 3. في حال اكتشاف جدار جديد، يتم تحديث المسافات بواسطة Flood Fill
      if (newWallDiscovered) {
        floodFill(currentX, currentY);
      }

      // 4. اختيار الخلية المجاورة صاحبة أقل مسافة للتحرك إليها
      int bestX = currentX;
      int bestY = currentY;
      int minVal = dist[currentX][currentY];
      Direction targetDir = currentDir;

      // فحص الشمال
      if (!memTopWall[currentX][currentY] && currentY > 0 && dist[currentX][currentY - 1] < minVal) {
        minVal = dist[currentX][currentY - 1];
        bestX = currentX;
        bestY = currentY - 1;
        targetDir = DIR_NORTH;
      }
      // فحص الشرق
      if (!memRightWall[currentX][currentY] && currentX < MAZE_SIZE - 1 && dist[currentX + 1][currentY] < minVal) {
        minVal = dist[currentX + 1][currentY];
        bestX = currentX + 1;
        bestY = currentY;
        targetDir = DIR_EAST;
      }
      // فحص الجنوب
      if (!memBottomWall[currentX][currentY] && currentY < MAZE_SIZE - 1 && dist[currentX][currentY + 1] < minVal) {
        minVal = dist[currentX][currentY + 1];
        bestX = currentX;
        bestY = currentY + 1;
        targetDir = DIR_SOUTH;
      }
      // فحص الغرب
      if (!memLeftWall[currentX][currentY] && currentX > 0 && dist[currentX - 1][currentY] < minVal) {
        minVal = dist[currentX - 1][currentY];
        bestX = currentX - 1;
        bestY = currentY;
        targetDir = DIR_WEST;
      }

      // 5. تدوير الروبوت فيزيائياً باتجاه الخلية المختارة
      int dirDiff = (targetDir - currentDir + 4) % 4;
      if (dirDiff == 1) {
        turnRight(BASE_SPEED);
      } else if (dirDiff == 2) {
        ReturnBack(BASE_SPEED);
      } else if (dirDiff == 3) {
        turnLeft(BASE_SPEED);
      }
      currentDir = targetDir;

      // 6. التقدم للأمام خلية واحدة
      moveDistance(BASE_SPEED);

      // 7. تحديث الإحداثيات وتسجيلها في السجل
      currentX = bestX;
      currentY = bestY;
      recordStep(currentX, currentY);

      delay(100);
      break;
    }

    // --- مرحلة حساب أقصر مسار (Solving) ---
    case STATE_SOLVING: {
      Serial.println(F("Computing Shortest Path (Loop Reduction)..."));
      computeShortestPath();

      Serial.print(F("Path computed with length: "));
      Serial.println(getShortestPathLength());

      Serial.println(F("Prepare for Speed Run in 5 seconds..."));
      delay(5000); // مهلة للتحضير أو إعادة الروبوت يدوياً لنقطة البداية

      currentState = STATE_SPEED_RUN;
      break;
    }

    // --- مرحلة الجري السريع (Speed Run) ---
    case STATE_SPEED_RUN: {
      Serial.println(F("Running Full Speed Simulation along the computed path..."));
      // تنفيذ المسار بسرعة أعلى (مثلاً زيادة سرعة الحركة إلى 170 أو BASE_SPEED)
      runFullSimulation(BASE_SPEED + 20);

      currentState = STATE_FINISHED;
      break;
    }

    // --- مرحلة الانتهاء والتوقف التام ---
    case STATE_FINISHED: {
      stopMotors();
      // تشغيل إشارة انتهاء أو البقاء في وضع السكون
      delay(1000);
      break;
    }
  }
}