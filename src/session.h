#ifndef _SESSION_H_
#define _SESSION_H_
#include <Arduino.h>

bool isSessionStarted = false;
bool sessionTimeoutFlag = false;
float sessionSubTotal = 0;

void startSession();
void coinCollected(uint8_t coinIdx);
void forceEndSession();
void sessionTick();
#endif