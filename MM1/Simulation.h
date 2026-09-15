//HITHAM
#ifndef SIMULATION_H
#define SIMULATION_H
#include <Arduino.h>
#include "Solving.h"
#include "WallDet.h"

// المتغيرات الخاصة بالمسار الحالي
extern int currentSimulationIndex;

void resetSimulation();
bool stepSimulation(int speedVal);
bool isSimulationFinished();
void runFullSimulation(int speedVal);
#endif