#include "WallDet.h"
#include "FloodFill.h"
#include "Definitions.h"

// إحداثيات واتجاه الروبوت الابتدائي
int currentX = 0;
int currentY = 0;
Direction currentDir = DIR_NORTH;

VL53L0X tofSensor;

 //tof
void initSensors() {
  Wire.begin();
  tofSensor.setTimeout(500);
  if (!tofSensor.init()) {
    Serial.println(F("Failed to detect tof!"));
  }
  tofSensor.startContinuous();
}

int getTofDistance() {
  uint16_t distance = tofSensor.readRangeContinuousMillimeters();
  if (tofSensor.timeoutOccurred()) {
    return 9999;
  }
  return (int)distance;
}

//Ir read
int readIRLeftRaw() {
    return analogRead(IR_LEFT_PIN);
}

int readIRRightRaw() {
    return analogRead(IR_RIGHT_PIN);
}

int readIRFrontRaw() {
    return analogRead(IR_FRONT_PIN);
}

bool isLeftWallDetected() {
  return (readIRLeftRaw() > IR_SIDE_WALL_THRESHOLD);
}


bool isRightWallDetected() {
  return (readIRRightRaw() > IR_SIDE_WALL_THRESHOLD);
}

bool isFrontWallDetected() {
  return (getTofDistance() <= FRONT_WALL_THRESHOLD_MM);
}

bool senseWalls() {
  bool frontWall = isFrontWallDetected();
  bool leftWall  = isLeftWallDetected();
  bool rightWall = isRightWallDetected();

  bool absNorth = false;
  bool absEast  = false;
  bool absSouth = false;
  bool absWest  = false;

  // تحويل اتجاهات الروبوت النسبية للاتجاهات الجغرافية
  switch (currentDir) {
    case DIR_NORTH:
      absNorth = frontWall;
      absEast  = rightWall;
      absWest  = leftWall;
      break;
    case DIR_EAST:
      absEast  = frontWall;
      absSouth = rightWall;
      absNorth = leftWall;
      break;
    case DIR_SOUTH:
      absSouth = frontWall;
      absWest  = rightWall;
      absEast  = leftWall;
      break;
    case DIR_WEST:
      absWest  = frontWall;
      absNorth = rightWall;
      absSouth = leftWall;
      break;
  }

  bool wallDiscovered = false;

  // تحديث الجدار الشمالي والخلية التي تعلوه
  if (absNorth && !memTopWall[currentX][currentY]) {
    memTopWall[currentX][currentY] = true;
    if (currentY > 0) memBottomWall[currentX][currentY - 1] = true;
    wallDiscovered = true;
  }

  // تحديث الجدار الجنوبي والخلية التي أسفله
  if (absSouth && !memBottomWall[currentX][currentY]) {
    memBottomWall[currentX][currentY] = true;
    if (currentY < MAZE_SIZE - 1) memTopWall[currentX][currentY + 1] = true;
    wallDiscovered = true;
  }

  // تحديث الجدار الغربي والخلية المجاورة يساراً
  if (absWest && !memLeftWall[currentX][currentY]) {
    memLeftWall[currentX][currentY] = true;
    if (currentX > 0) memRightWall[currentX - 1][currentY] = true;
    wallDiscovered = true;
  }

  // تحديث الجدار الشرقي والخلية المجاورة يميناً
  if (absEast && !memRightWall[currentX][currentY]) {
    memRightWall[currentX][currentY] = true;
    if (currentX < MAZE_SIZE - 1) memLeftWall[currentX + 1][currentY] = true;
    wallDiscovered = true;
  }

  return wallDiscovered;
}


//printMemoryDump 

void printMemoryDump() {
  Serial.println(F("ROBOT INTERNAL MEMORY DUMP"));
  Serial.println(F("Format: [Distance | NESW Walls] -> (N=North, E=East, S=South, W=West, '.'=Open)"));

  char buffer[32];
  for (int y = 0; y < MAZE_SIZE; y++) {
    for (int x = 0; x < MAZE_SIZE; x++) {
      char n = memTopWall[x][y] ? 'N' : '.';
      char e = memRightWall[x][y] ? 'E' : '.';
      char s = memBottomWall[x][y] ? 'S' : '.';
      char w = memLeftWall[x][y] ? 'W' : '.';

      if (x == currentX && y == currentY) {
        snprintf(buffer, sizeof(buffer), "[*%02d:%c%c%c%c*] ", dist[x][y], n, e, s, w);
      } else {
        snprintf(buffer, sizeof(buffer), "[%02d:%c%c%c%c] ", dist[x][y], n, e, s, w);
      }
      Serial.print(buffer);
    }
    Serial.println();
  }
}