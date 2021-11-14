#include "menu.h"

extern Bounce leftBounce;
extern Bounce rightBounce;
extern Bounce downBounce;
extern Bounce topBounce;
extern Bounce selectBounce;

enum MenuStateEnum
{
    mainMenu,
    additionalMenu,
    calibraionMenu,
};

MenuStateEnum menuState = mainMenu;

//0 - count menu
//1 - price menu,
//2 - percent menu,
//3 - weight menu

#define ADD_MENU_CNT 4
uint8_t additionalState = 0;
extern LiquidCrystal_I2C lcd;


void menuTick()
{

    switch (menuState)
    {
    case mainMenu:
        /* code */
        if (downBounce.fell())
        {
            //TODO draw additional menu
            menuState = additionalMenu;
        }

        if (topBounce.fell())
        {
            //TODO draw additional menu
            menuState = calibraionMenu;
            menuInitcalibrationFlag = true;
        }

        break;
    case additionalMenu:
        if (topBounce.fell())
        {
            //TODO draw main menu
            menuState = mainMenu;
        }
        if (rightBounce.fell())
        {
            additionalState = (additionalState + 1) % ADD_MENU_CNT;
            //TODO draw addit
        }
        if (leftBounce.fell())
        {
            additionalState = (additionalState - 1 + ADD_MENU_CNT) % ADD_MENU_CNT;
            //TODO draw addit
        }

        break;

    case calibraionMenu:
        break;
    }
}

void menu_display_user_greet(uint8_t userChangedId)
{
    lcd.setCursor(3, 0);
    lcd.print("Hello");
    lcd.setCursor(3, 1);
    lcd.print(userNames[userChangedId]);
    delay(DISPLAY_MESSAGE_DELAY);
}

void menu_display_user_leave(uint8_t userChangedId)
{
    lcd.setCursor(3, 0);
    lcd.print("Goodbye");
    lcd.setCursor(3, 1);
    lcd.print(userNames[userChangedId]);
    delay(DISPLAY_MESSAGE_DELAY);
}

void menu_main_screen_top(uint8_t userChangedId, int8_t currentCurrency, bool drawbackChanged)
{
    char top_msg[DISPLAY_COLUMNS + 1];
    sprintf(top_msg, "%d  %s  %s", userChangedId, userNames[userChangedId], currency[currentCurrency]);
    lcd.setCursor(3, 0);
    lcd.print(top_msg);
}

void menu_main_screen_pure(UserStruct data, int8_t currentCurrency, bool drawbackChanged)
{
    char down_msg[DISPLAY_COLUMNS + 1];
    sprintf(down_msg, "@%.2f   %s", data.subtotal[currentCurrency], drawbackChanged ? "DB" : "");
    lcd.setCursor(3, 1);
    lcd.print(down_msg);

    delay(DISPLAY_MESSAGE_DELAY);
}

void menu_main_screen_sign(UserStruct data, int8_t currentCurrency, bool drawbackChanged, float currentValue, bool sign)
{
    char down_msg[DISPLAY_COLUMNS + 1];
    sprintf(down_msg, "@%.2f%c%.2f  %s", data.subtotal[currentCurrency], sign ? '+' : '-', currentValue, drawbackChanged ? "DB" : "");
    lcd.setCursor(3, 1);
    lcd.print(down_msg);

    delay(DISPLAY_MESSAGE_DELAY);
}

/////////////////////////
// additional info START
/////////////////////////
void menu_additional_info_quantity(UserStruct data)
{
    char value[10];
    char top_msg[60];
    int pos = 0;
    pos += sprintf(&top_msg[pos], "# ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%2d ", uah_value[i] < 0 ? uah_value[i] * 100 : uah_value[i]);
        pos += sprintf(&top_msg[pos], "%s", value);
    }

    char down_msg[60];
    pos = 0;
    pos += sprintf(&top_msg[pos], "@ ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%02d ", data.uah_quantity[i] < 0 ? data.uah_quantity[i] * 100 : data.uah_quantity[i]);
        pos += sprintf(&down_msg[pos], "%s", value);
    }

    //lcd.setCursor(15, 0);
    for (int i = 0; i < strlen(top_msg); i++)
    {
        //lcd.scrollDisplayLeft();
        lcd.setCursor(15 - i, 0);
        lcd.print(top_msg[i]);

        lcd.setCursor(15 - i, 1);
        lcd.print(down_msg[i]);

        delay(carousel_delay);
    }
}

void menu_additional_info_subtotal(UserStruct data)
{
    char value[10];
    char top_msg[60];
    int pos = 0;
    pos += sprintf(&top_msg[pos], "# ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%2.2f ", uah_value[i]);
        pos += sprintf(&top_msg[pos], "%s", value);
    }

    char down_msg[60];
    pos = 0;
    pos += sprintf(&down_msg[pos], "@ ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%03.2f ", uah_value[i] * data.uah_quantity[i]);
        pos += sprintf(&down_msg[pos], "%s", value);
    }

    //lcd.setCursor(15, 0);
    for (int i = 0; i < strlen(top_msg); i++)
    {
        //lcd.scrollDisplayLeft();
        lcd.setCursor(15 - i, 0);
        lcd.print(top_msg[i]);

        lcd.setCursor(15 - i, 1);
        lcd.print(down_msg[i]);

        delay(carousel_delay);
    }
}

void menu_additional_info_percent(UserStruct data)
{
    char value[10];
    char top_msg[60];
    int pos = 0;
    pos += sprintf(&top_msg[pos], "# ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%2.2f ", uah_value[i]);
        pos += sprintf(&top_msg[pos], "%s", value);
    }

    char down_msg[60];
    pos = 0;
    pos += sprintf(&down_msg[pos], "@ ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%02.1f% ", ((uah_value[i] * data.uah_quantity[i]) / data.subtotal[0]) * 100);
        pos += sprintf(&down_msg[pos], "%s", value);
    }

    //lcd.setCursor(15, 0);
    for (int i = 0; i < strlen(top_msg); i++)
    {
        //lcd.scrollDisplayLeft();
        lcd.setCursor(15 - i, 0);
        lcd.print(top_msg[i]);

        lcd.setCursor(15 - i, 1);
        lcd.print(down_msg[i]);

        delay(carousel_delay);
    }
}

void menu_additional_info_weight(UserStruct data)
{
    char value[10];
    char top_msg[60];
    int pos = 0;
    pos += sprintf(&top_msg[pos], "# ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%2.2f ", uah_value[i]);
        pos += sprintf(&top_msg[pos], "%s", value);
    }

    char down_msg[60];
    pos = 0;
    pos += sprintf(&down_msg[pos], "@ ");
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        sprintf(value, "%03.1fg ", uah_coin_weight[i] * data.uah_quantity[i]);
        pos += sprintf(&down_msg[pos], "%s", value);
    }

    //lcd.setCursor(15, 0);
    for (int i = 0; i < strlen(top_msg); i++)
    {
        //lcd.scrollDisplayLeft();
        lcd.setCursor(15 - i, 0);
        lcd.print(top_msg[i]);

        lcd.setCursor(15 - i, 1);
        lcd.print(down_msg[i]);

        delay(carousel_delay);
    }
}
/////////////////////////
// additional info FINISH
/////////////////////////

void menu_update_data()
{
    return;
}

void menu_change_drawback_sign()
{
    return;
}

void menu_navigate_to_main()
{
    return;
}

void menuDisplaySessionSubtotal()
{
    return;
}

void menuDisplayIncorrectCoin(int8_t lastCoinIdx, bool lastWeightCorrect)
{
    lcd.setCursor(3, 0);
    lcd.print("LAST COIN");
    lcd.setCursor(3, 1);
    lcd.print("IS INCORRECT");
    delay(DISPLAY_MESSAGE_DELAY);
}

void menu_session_finished(UserStruct data)
{
    lcd.setCursor(3, 0);
    lcd.print("YOU LIGHTER OF");

    float amount = 0;
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        amount += uah_coin_weight[i] * data.uah_quantity[i];
    }

    for (uint8_t i = 0; i < RUB_AMOUNT; i++)
    {
        amount += rub_coin_weight[i] * data.uah_quantity[i];
    }

    for (uint8_t i = 0; i < USD_AMOUNT; i++)
    {
        amount += usd_coin_weight[i] * data.uah_quantity[i];
    }

    for (uint8_t i = 0; i < EUR_AMOUNT; i++)
    {
        amount += eur_coin_weight[i] * data.uah_quantity[i];
    }

    char value[8];
    sprintf(value, "%03.1fg!", amount);
    lcd.setCursor(3, 1);
    lcd.print(value);
    delay(DISPLAY_MESSAGE_DELAY);
}


void menuCalibrationDisplayNominal(uint8_t index, int8_t currentCurrencyIdx) {
    lcd.setCursor(3, 0);
    lcd.print("Calibration");
    lcd.setCursor(3, 1);

    char value[DISPLAY_COLUMNS + 1];
    switch (currentCurrencyIdx)
    {
    case 0:
        sprintf(value, "%d        UAH", uah_value[index]);
        break;
    case 1:
        sprintf(value, "%d        RUB", rub_value[index]);
        break;
    case 2:
        sprintf(value, "%d        USD", usd_value[index]);
        break;
    case 3:
        sprintf(value, "%d        EUR", eur_value[index]);
        break;
    default:
        break;
    }
    lcd.print(value);
    delay(DISPLAY_MESSAGE_DELAY);
}
