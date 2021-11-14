#include "session.h"
#include "main.h"
#include "user.h"
#include <Bounce2.h>

uint64_t sessionLastAction = 0;
UserStruct sessionStruct;
extern Bounce drawbackBounce;

extern UserStruct *currentUser = nullptr;

void startSession()
{
    isSessionStarted = true;
    sessionSubTotal = 0;
}

void coinCollected(uint8_t coinIdx)
{
    if (!isSessionStarted)
    {
        startSession();
    }
    sessionLastAction = millis();
    sessionSubTotal += currencyValues[currentCurrencyIdx][coinIdx] * (drawbackBounce.read() ? 1 : -1);
}

void EndSession()
{
    isSessionStarted = false;
    sessionTimeoutFlag = true;
    isSessionStarted = false;

    currentUser->subtotal[currentCurrencyIdx] += sessionSubTotal;

    sessionSubTotal = 0;
}

void sessionTick()
{
    if (isSessionStarted)
    {
        if (millis() - sessionLastAction > SESSION_TIMEOUT)
        {
            EndSession();
        }
    }
}

void finishSession()
{
}
