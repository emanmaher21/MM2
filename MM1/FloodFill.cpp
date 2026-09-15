//MAHER
#include "FloodFill.h"
#include "Definitions.h"

// تعريف مصفوفات الذاكرة المشتركة
int dist[MAZE_SIZE][MAZE_SIZE];
bool memTopWall[MAZE_SIZE][MAZE_SIZE];
bool memBottomWall[MAZE_SIZE][MAZE_SIZE];
bool memLeftWall[MAZE_SIZE][MAZE_SIZE];
bool memRightWall[MAZE_SIZE][MAZE_SIZE];

// هيكل بسيط لتمثيل إحداثيات الخلية
struct Coord {
  int x;
  int y;
};

// طابور دائري ثابت (Circular Queue) مخصص للميكروكنترولر بحجم 256 خلية
#define QUEUE_MAX 256
static Coord queue[QUEUE_MAX];
static int qHead = 0;
static int qTail = 0;

static void qPush(int x, int y) {
  queue[qTail].x = x;
  queue[qTail].y = y;
  qTail = (qTail + 1) % QUEUE_MAX;
}

static Coord qPop() {
  Coord c = queue[qHead];
  qHead = (qHead + 1) % QUEUE_MAX;
  return c;
}

static bool qIsEmpty() {
  return qHead == qTail;
}

/* 
 * دالة تهيئة مصفوفة المسافات (Manhattan Distances):
 * تقوم بحساب البعد الافتراضي لكل خلية عن مركز المتاهة (الخلايا 7 و 8)[cite: 1]
 * وتحديد الجدران الخارجية للمتاهة كجدران مغلقة مبدئياً[.
 */
void initDistances() {
  for (int i = 0; i < MAZE_SIZE; i++) {
    for (int j = 0; j < MAZE_SIZE; j++) {
      // حساب مسافة مانهاتن بالنسبة للمركز 7 و 8
      int dx = min(abs(i - 7), abs(i - 8)); 
      int dy = min(abs(j - 7), abs(j - 8)); 
      dist[i][j] = dx + dy; 

      // ضبط الحدود الخارجية كجدران مؤكدة
      memTopWall[i][j] = (j == 0);
      memBottomWall[i][j] = (j == MAZE_SIZE - 1);
      memLeftWall[i][j] = (i == 0);
      memRightWall[i][j] = (i == MAZE_SIZE - 1);
    }
  }
}

/* 
 * دالة فحص وصول الروبوت للمركز:
 * تتحقق مما إذا كانت الخلية الحالية تقع ضمن منطقة الهدف المكونة من 4 خلايا في المنتصف.
 */
bool isAtCenter(int x, int y) {
  return (x == 7 || x == 8) && (y == 7 || y == 8); 
}

/* 
 * دالة فحص أقل قيمة للمجاورين:
 * تبحث في الاتجاهات الأربعة (شمال، جنوب، غرب، شرق)
 * وتعيد أقل قيمة مسافة لخلية مجاورة بشرط عدم وجود جدار فاصل بينهماcite: 1].
 */
int getMinOpenNeighbor(int x, int y) {
  int minVal = 9999; // قيمة ابتدائية كبرى

  // فحص اتجاه الشمال
  if (!memTopWall[x][y] && y > 0) { 
    minVal = min(minVal, dist[x][y - 1]); 
  }
  // فحص اتجاه الجنوب
  if (!memBottomWall[x][y] && y < MAZE_SIZE - 1) { 
    minVal = min(minVal, dist[x][y + 1]); 
  }
  // فحص اتجاه الغرب
  if (!memLeftWall[x][y] && x > 0) { 
    minVal = min(minVal, dist[x - 1][y]); 
  }
  // فحص اتجاه الشرق
  if (!memRightWall[x][y] && x < MAZE_SIZE - 1) { 
    minVal = min(minVal, dist[x + 1][y]); 
  }

  return minVal; 
}

/* 
 * دالة إضافة الجيران المتاحين إلى الطابور:
 * تقوم بوضع جميع الخلايا المجاورة التي لا يفصلها جدار عن الخلية الحالية داخل الطابورcite: 1].
 */
void pushOpenNeighbors(int x, int y) {
  if (!memTopWall[x][y] && y > 0) qPush(x, y - 1); 
  if (!memBottomWall[x][y] && y < MAZE_SIZE - 1) qPush(x, y + 1); 
  if (!memLeftWall[x][y] && x > 0) qPush(x - 1, y); 
  if (!memRightWall[x][y] && x < MAZE_SIZE - 1) qPush(x + 1, y); 
}

/* 
 * خوارزمية فلوود فيل (Flood Fill Algorithm):
 * تبدأ من موقع الروبوت الحالي؛ وفي حال لم تكن مسافة الخلية الحالية تساوي (أقل مسافة لجيرانها + 1)،
 * يتم تحديث المسافة ونشر التحديث للخلايا المجاورة عبر الطابور حتى تستقر جميع مسارات المتاهةcite: 1].
 */
void floodFill(int startX, int startY) {
  qHead = 0;
  qTail = 0;
  qPush(startX, startY); 

  while (!qIsEmpty()) { 
    Coord curr = qPop(); 
    int cx = curr.x; 
    int cy = curr.y; 

    // تخطي خلايا الهدف بالمركز لتبقى قيمتها ثابتة كوجهة نهائية
    if (isAtCenter(cx, cy)) { 
      continue; 
    }

    int minNeighbor = getMinOpenNeighbor(cx, cy); 

    // إذا كانت المسافة الحالية غير متطابقة مع التدرج المنطقي
    if (dist[cx][cy] != minNeighbor + 1) { 
      dist[cx][cy] = minNeighbor + 1; // تحديث المسافة
      pushOpenNeighbors(cx, cy);      // دفع الجيران لإعادة تقييمهم
  }
}
}