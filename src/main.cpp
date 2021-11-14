#include <Arduino.h>
#include "EEPROMex.h"
#include "LiquidCrystal_I2C.h"
#include <Bounce2.h>

#include "main.h"
#include "menu.h"
#include "user.h"
#include "session.h"
#include "sensor.h"
#include "calibration.h"

// quantity of coins
uint16_t empty_signal;               // level of empty signal
uint64_t standby_timer, reset_timer; // timers

LiquidCrystal_I2C lcd(0x27, 16, 2);
Bounce sorterBounce = Bounce();
Bounce drawbackBounce = Bounce();

Bounce leftBounce = Bounce();
Bounce rightBounce = Bounce();
Bounce downBounce = Bounce();
Bounce topBounce = Bounce();
Bounce selectBounce = Bounce();

UserStruct FakeUser;
UserStruct users[USER_AMOUNT];
UserStruct *currentUser = nullptr;


extern bool coinTravledFlag;
extern uint8_t lastCoinIdx;

void setup()
{
  Serial.begin(115200);
  for (uint8_t i = 0; i < USER_AMOUNT; i++)
  {
    eeprom_user_read(&users[i], i);
  }
  currentCurrencyIdx = eeprom_calibration_last_currency_read();

  //loadEeprom(); //TODO are we read all requiered date?

  //GPIO init
  pinMode(IR_sorter, INPUT);
  sorterBounce.attach(IR_sorter);
  sorterBounce.interval(DEBOUNCE_TIME);

  pinMode(IR_drawback, INPUT);
  sorterBounce.attach(IR_drawback);
  sorterBounce.interval(DEBOUNCE_TIME);

  pinMode(LEFT_BUTTON, INPUT);
  leftBounce.attach(LEFT_BUTTON);
  leftBounce.interval(DEBOUNCE_TIME);

  pinMode(RIGHT_BUTTON, INPUT);
  rightBounce.attach(RIGHT_BUTTON);
  rightBounce.interval(DEBOUNCE_TIME);

  pinMode(TOP_BUTTON, INPUT);
  topBounce.attach(TOP_BUTTON);
  topBounce.interval(DEBOUNCE_TIME);

  pinMode(DOWN_BUTTON, INPUT);
  downBounce.attach(DOWN_BUTTON);
  downBounce.interval(DEBOUNCE_TIME);

  pinMode(SELECT_BUTTON, INPUT);
  selectBounce.attach(SELECT_BUTTON);
  selectBounce.interval(DEBOUNCE_TIME);

  //TODO add other pins

  //init
  lcd.init();
  lcd.backlight();
  //TODO eepron init
  sensorInit();
  userInit();
  //TODO should we init something other?
}

void loop()
{
  bool sorterChanged = sorterBounce.update();
  bool drawbackChanged = drawbackBounce.update();

  bool topChanged = topBounce.update();
  bool downChanged = downBounce.update();
  bool leftChanged = leftBounce.update();
  bool rightChanged = rightBounce.update();
  bool selectChanged = selectBounce.update();

  menuTick();
  sessionTick();

  //flags
  if (coinTravledFlag)
  {
    coinTravledFlag = false;

    if (lastCoinIdx == -1 || !lastWeightCorrect)
    {
      menuDisplayIncorrectCoin(lastCoinIdx, lastWeightCorrect);
    }
    else
    {
      int8_t add = (drawbackBounce.read() ? 1 : -1);
      //coin is correct
      switch (currentCurrencyIdx)
      {
      case UAH:
        currentUser->uah_quantity[lastCoinIdx] += add;
        break;
      case RUB:
        currentUser->rub_quantity[lastCoinIdx] += add;
        break;
      case EUR:
        currentUser->eur_quantity[lastCoinIdx] += add;
        break;
      case USD:
        currentUser->usd_quantity[lastCoinIdx] += add;
        break;
      }
      coinCollected(lastCoinIdx);
      menu_update_data(); //update user money count and/or active session
    }
  }

  if (userChangedFlag)
  {
    userChangedFlag = false;
    if (sorterBounce.read())
    {
      //if sorter is attahed
      //ignore
    }
    else
    {
      //if no sorter

      currentUser = &users[userChangedId];
      if (userChangedId == 0)
      {
        //no user
        menu_display_user_leave(userChangedId);

        memset(&FakeUser, 0, sizeof(FakeUser)); //CLEAR FAKE USER
        currentUser = &FakeUser;
      }
      else
      {
        menu_display_user_greet(userChangedId);
      }
    }

    //TODO handle user change
  }

  if (sorterChanged)
  {
    //TODO handle sorter attach/detach
    if (userChangedId == 0 && !sorterBounce.read())
    {
      memset(&FakeUser, 0, sizeof(FakeUser)); //CLEAR FAKE USER
      currentUser = &FakeUser;
    }
    else if (sorterBounce.read())
    {
      currentUser = &users[0];
    }
    else
    {
      currentUser = &users[userChangedId];
      menu_display_user_greet(userChangedId);
    }
  }

  if (drawbackChanged)
  {
    //TODO hadle drawback change
    menu_change_drawback_sign();
  }

  if (sessionTimeoutFlag)
  {
    //TODO handle session timeout
    
    //TODO save eeprom
    //TODO display upd main screen
    menuDisplaySessionSubtotal();
  }

  if(menuInitcalibrationFlag){
    menuInitcalibrationFlag = false;
    calibration();
    menu_navigate_to_main();
  }
}

int get_value_index(int currency, float value)
{
  int amount = 0;
  float *values = {};
  switch (currency)
  {
  case 0:
    amount = UAH_AMOUNT;
    values = uah_value;
    break;
  case 1:
    amount = RUB_AMOUNT;
    values = rub_value;
    break;
  case 2:
    amount = USD_AMOUNT;
    values = usd_value;
    break;
  case 3:
    amount = EUR_AMOUNT;
    values = eur_value;
    break;
  default:
    amount = -1;
    break;
  }

  if (amount == -1)
  {
    // std::cout << "Error: get_value_index" << std::endl;
  }

  for (int i = 0; i < amount; i++)
  {
    if (values[i] == value)
      return i;
  }
}