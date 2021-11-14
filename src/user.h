#ifndef _USER_H_
#define _USER_H_
#include "main.h"
#include "EEPROMex.h"

bool userChangedFlag = false;
bool userChangeStarted = false;
uint8_t userChangedId = 0;

// 43
struct UserStruct
{
    // SORTER_OFFSET 0
    // USER1_OFFSET 43
    // USER2_OFFSET 86
    // USER3_OFFSET 129
    // USER4_OFFSET 172
    uint8_t user_offset;
    uint8_t uah_quantity[UAH_AMOUNT];
    uint8_t rub_quantity[RUB_AMOUNT];
    uint8_t eur_quantity[EUR_AMOUNT];
    uint8_t usd_quantity[USD_AMOUNT];
    float subtotal[CURRENCY_AMOUNT];
};

void userInit();


void eeprom_user_init(UserStruct data);
void eeprom_user_save(UserStruct data);
void eeprom_user_update(UserStruct data, uint8_t currency, float value);
uint8_t eeprom_user_quantity_read(UserStruct data, uint8_t currency, uint8_t index);
float eeprom_user_subtotal_read(UserStruct data, uint8_t currency, uint8_t index);
float eeprom_user_read(UserStruct* data, uint8_t userIdx);

#endif
