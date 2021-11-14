#ifndef _MENU_H_
#define _MENU_H_
#include "main.h"
#include <Bounce2.h>
#include <string.h>
#include "user.h"

/*
################
################

//main screen +++

1 USERNAME   UAH
@200.00       DB

2 USERNAME   UAH 
@200.00+1.00  DB

1 USERNAME   UAH 
@200.00-1.00  DB

//after session finished +++
YOU LIGHTER OF
125.4g!

//additional info +++
#10 50 _1 _2 _5 10
@05 05 05 05 05 05

# 00.10   0.50  01.00  02.00  05.00  10.00
@123.10 123.24 782.21 423.12 154.67 120.64

# 00.10 0.50  01.00 02.00 05.00 10.00
@ 50.1% 50.1% 50.1% 50.1% 50.1% 50.1%

# 00.10   0.50  01.00  02.00  05.00  10.00
@123.1g 123.2g 782.2g 423.1g 154.6g 120.6g


//sorter attached

 SORTER ATTACHED
 SORTER DETACHED

//drawback switched

    TOP_UP
     MODE

    BORROW
     MODE

//user switched +++
//no user
Goodbye
username

hello
username

//herovaya moneta +++
LAST COIN
IS INCORRECT

WRONG DIAMETER
WRONG SPEED


//if sorter attached

CHECK 1 SLOT
(0.10 UAH)


*/
#include "main.h"
#include "LiquidCrystal_I2C.h"
#include "user.h"

bool menuInitcalibrationFlag = false;

void menuTick();

void onLeftClick();
void onRightClick();
void onUpClick();
void onDownClick();
void onCenterClick();

void menu_display_user_greet(uint8_t userChangedId);
void menu_display_user_leave(uint8_t userChangedId);
void menu_main_screen_top(uint8_t userChangedId, int8_t currentCurrency, bool drawbackChanged);
void menu_main_screen_pure(UserStruct data, int8_t currentCurrency, bool drawbackChanged);
void menu_main_screen_sign(UserStruct data, int8_t currentCurrency, bool drawbackChanged, float currentValue, bool sign);

void menu_additional_info_quantity(UserStruct data);
void menu_additional_info_subtotal(UserStruct data);
void menu_additional_info_percent(UserStruct data);
void menu_additional_info_weight(UserStruct data);
// empty
void menu_update_data();
void menu_change_drawback_sign();
void menu_navigate_to_main();
void menuDisplaySessionSubtotal();
// ----
void menuCalibrationDisplayNominal(uint8_t index, int8_t currentCurrencyIdx);
void menuDisplayIncorrectCoin(int8_t lastCoinIdx, bool lastWeightCorrect);
void menu_session_finished(UserStruct data);
#endif