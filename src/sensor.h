#ifndef _SENSOR_H_
#define _SENSOR_H_
#include <Arduino.h>
#include "main.h"
#include <Bounce2.h>

bool calibrationInProgress;
bool coinTravledFlag;
bool lastWeightCorrect;
uint8_t lastCoinIdx;

void sensorInit();
void sensorTick();
void calibration();
#endif
