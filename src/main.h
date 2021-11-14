#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>

//calibration values
#define ADC_CLEARENCE   10
#define TIME_CLEARENCE  100
#define DEBOUNCE_TIME   100
#define USER_DETECT_TIME 500

//buffers
#define ADC_BUFF_LEN 3
//delays setup
#define stb_time 10000    // idle time to sleep
#define carousel_delay 500

#define SESSION_TIMEOUT 5000
#define DISPLAY_MESSAGE_DELAY 4000

//pinout setup

//uart 0
//uart 1
//display a4
//display a5

#define RIGHT_BUTTON    8
#define LEFT_BUTTON     9
#define TOP_BUTTON      10
#define DOWN_BUTTON     11
#define SELECT_BUTTON   12

#define IR_analogue A7
#define IR_first    2
#define IR_second   3
#define IR_sorter   4
#define IR_drawback 5

#define IR_user0 A1     //all user pins must be in PCINT1_vect
#define IR_user1 A2
#define IR_user2 A3

// user offset
#define SORTER_OFFSET 0
#define USER1_OFFSET (SORTER_OFFSET + 31) // 31
#define USER2_OFFSET (USER1_OFFSET + 31) // 62
#define USER3_OFFSET (USER2_OFFSET + 31) // 93
#define USER4_OFFSET (USER3_OFFSET + 31) // 124

// calibration offset
#define CURRENCY_SIGNALS_OFFSET (USER4_OFFSET + 31) // 155
#define CURRENCY_TIMES_OFFSET (CURRENCY_SIGNALS_OFFSET + 52) // 207
#define LAST_CURRENCY_OFFSET (CURRENCY_TIMES_OFFSET + 52) // 259

// menu type offset
#define MENU_AMOUNT_OFFSET (LAST_CURRENCY_OFFSET + 1) // 489
#define MENU_PERCENT_OFFSET (MENU_AMOUNT_OFFSET + 1) // 490
#define MENU_WEIGHT_OFFSET (MENU_PERCENT_OFFSET + 1) // 491

// menu type
#define MENU_AMOUNT 0
#define MENU_PERCENT 1
#define MENU_WEIGHT 2

#define DISPLAY_COLUMNS 17 // ???

//user setup
#define USER_AMOUNT 5
char *userNames[USER_AMOUNT] = {"Sorter", "Ann", "Victor", "Andrey", "Danil"};

// currency setup
#define CURRENCY_AMOUNT 4

// currency type
#define UAH 0
#define RUB 1
#define USD 2
#define EUR 3

#define UAH_AMOUNT 6  // amount of UAH coins
#define RUB_AMOUNT 6  // amount of RUB coins
#define USD_AMOUNT 6  // amount of USD coins
#define EUR_AMOUNT 8  // amount of EUR coins

uint8_t currencyLen[CURRENCY_AMOUNT] = {
    UAH_AMOUNT,
    RUB_AMOUNT,
    USD_AMOUNT,
    EUR_AMOUNT,
};



// https://bank.gov.ua/ua/uah/obig-coin
float uah_coin_weight[UAH_AMOUNT] = {1.7, 4.2, 3.3, 4.0, 5.2, 6.4};
// https://bankirsha.com/ves-monet-i-banknot-banka-rossii.html
float rub_coin_weight[RUB_AMOUNT] = {1.95, 2.9, 3.25, 5.1, 6.45, 5.63};
// https://avocadoughtoast.com/weights-sizes-us-coins/
float usd_coin_weight[USD_AMOUNT] = {2.5, 5.0, 2.26, 5.67, 11.34, 8.1};
// https://en.wikipedia.org/wiki/Euro_coins
float eur_coin_weight[EUR_AMOUNT] = {2.3, 3.06, 3.92, 4.1, 5.74, 7.8, 7.5, 8.5};

float uah_value[UAH_AMOUNT] = {0.1, 0.5, 1.0, 2.0, 5.0, 10.0};              // UAH coin values
float rub_value[RUB_AMOUNT] = {0.1, 0.5, 1.0, 2.0, 5.0, 10.0};              // RUB coin values
float usd_value[USD_AMOUNT] = {0.01, 0.05, 0.1, 0.25, 0.5, 1.0};            // USD coin values
float eur_value[EUR_AMOUNT] = {0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0, 2.0};  // EUR coin values

char currency[CURRENCY_AMOUNT][4] = {"UAH", "RUB", "EUR", "USD"}; // currency

int get_value_index(int, float);

// global variables
// signal value for each coin size

uint16_t uah_signal[UAH_AMOUNT];
uint16_t rub_signal[RUB_AMOUNT];
uint16_t eur_signal[EUR_AMOUNT];
uint16_t usd_signal[USD_AMOUNT];

uint16_t uah_time[UAH_AMOUNT];
uint16_t rub_time[RUB_AMOUNT];
uint16_t eur_time[EUR_AMOUNT];
uint16_t usd_time[USD_AMOUNT];

float* currencyValues[CURRENCY_AMOUNT] = {
    uah_value,
    rub_value,
    usd_value,
    eur_value,
};

uint16_t* currencySignals[CURRENCY_AMOUNT] = {
    uah_signal,
    rub_signal,
    eur_signal,
    usd_signal,
};

uint16_t* currencyTimes[CURRENCY_AMOUNT] = {
    uah_time,
    rub_time,
    eur_time,
    usd_time,
};

int8_t currentCurrencyIdx = 0;

#endif
