#include "user.h"
#include "main.h"

//closed = high = 1
//opened = low = 0


uint64_t lastUserIrActivity = 0;
void userInit()
{
    //configure port ISR
    pinMode(IR_user0, INPUT);
    pinMode(IR_user1, INPUT);
    pinMode(IR_user2, INPUT);

    *digitalPinToPCMSK(IR_user0) |= bit (digitalPinToPCMSKbit(IR_user0));  // enable pin
    *digitalPinToPCMSK(IR_user1) |= bit (digitalPinToPCMSKbit(IR_user1));  // enable pin
    *digitalPinToPCMSK(IR_user2) |= bit (digitalPinToPCMSKbit(IR_user2));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(IR_user0)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(IR_user0)); // enable interrupt for the group

}

void userTick()
{
    if (userChangeStarted)
    {
        if (millis() - lastUserIrActivity > USER_DETECT_TIME)
        {
            userChangedFlag = true;
            userChangeStarted = false;

            userChangedId = digitalRead(IR_user0) | (digitalRead(IR_user0) << 1) | (digitalRead(IR_user0) << 2);
        }
    }
}

ISR(PCINT2_vect)
{
    userChangeStarted = true;
    lastUserIrActivity = millis();
}

void eeprom_user_init(UserStruct data, uint8_t offset)
{
    data.user_offset = offset;
    for (uint8_t i = 0; i < UAH_AMOUNT; i++)
    {
        data.uah_quantity[i] = 0;
        EEPROM.writeInt(offset + 1 + i, 0);
    }

    for (uint8_t i = 0; i < RUB_AMOUNT; i++)
    {
        data.rub_quantity[i] = 0;
        EEPROM.writeInt(offset + 7 + i, 0);
    }

    for (uint8_t i = 0; i < USD_AMOUNT; i++)
    {
        data.usd_quantity[i] = 0;
        EEPROM.writeInt(offset + 13 + i, 0);
    }

    for (uint8_t i = 0; i < EUR_AMOUNT; i++)
    {
        data.eur_quantity[i] = 0;
        EEPROM.writeInt(offset + 19 + i, 0);
    }

    for (uint8_t i = 0; i < CURRENCY_AMOUNT; i++)
    {
        data.subtotal[i] = 0.0;
        EEPROM.writeFloat(offset + 27 + i * 4, 0.0);
    }
}

void eeprom_user_save(UserStruct data, uint8_t offset) {
    EEPROM.writeInt(offset, data.user_offset);
    for (uint8_t i = 0; i < UAH_AMOUNT; i++) {
        EEPROM.writeInt(offset + 1 + i, data.uah_quantity[i]);
    }

    for (uint8_t i = 0; i < RUB_AMOUNT; i++) {
        EEPROM.writeInt(offset + 7 + i, data.rub_quantity[i]);
    }

    for (uint8_t i = 0; i < USD_AMOUNT; i++) {
        EEPROM.writeInt(offset + 13 + i, data.usd_quantity[i]);
    }

    for (uint8_t i = 0; i < EUR_AMOUNT; i++) {
        EEPROM.writeInt(offset + 19 + i, data.eur_quantity[i]);
    }

    for (uint8_t i = 0; i < CURRENCY_AMOUNT; i++) {
        EEPROM.writeFloat(offset + 27 + i * 4, data.subtotal[i]);
    }
}

void eeprom_user_update(UserStruct data, uint8_t currency, float value)
{
    uint8_t index = get_value_index(currency, value);
    switch (currency)
    {
    case 0:
        data.uah_quantity[index] += 1;
        EEPROM.updateInt(data.user_offset + 1 + index, data.uah_quantity[index]);
        
        data.subtotal[currency] += value;
        EEPROM.writeFloat(data.user_offset + 27 + currency * 4, value);
        break;
    case 1:
        data.rub_quantity[index] += 1;
        EEPROM.updateInt(data.user_offset + 7 + index, data.rub_quantity[index]);

        data.subtotal[currency] += value;
        EEPROM.writeFloat(data.user_offset + 27 + currency * 4, value);
        break;
    case 2:
        data.usd_quantity[index] += 1;
        EEPROM.updateInt(data.user_offset + 13 + index, data.usd_quantity[index]);

        data.subtotal[currency] += value;
        EEPROM.writeFloat(data.user_offset + 27 + currency * 4, value);
        break;
    case 3:
        data.eur_quantity[index] += 1;
        EEPROM.updateInt(data.user_offset + 19 + index, data.eur_quantity[index]);

        data.subtotal[currency] += value;
        EEPROM.writeFloat(data.user_offset + 27 + currency * 4, value);
        break;
    default:
        // std::cout << "Error: eeprom_user_update" << std::endl;
        break;
    }
}

uint8_t eeprom_user_quantity_read(UserStruct data, uint8_t currency, uint8_t index)
{
    switch (currency) {
    case 0:
        return EEPROM.readInt(data.user_offset + 1 + index);
    case 1:
        return EEPROM.readInt(data.user_offset + 7 + index);
    case 2:
        return EEPROM.readInt(data.user_offset + 13 + index);
    case 3:
        return EEPROM.readInt(data.user_offset + 19 + index);
    default:
        // std::cout << "Error: eeprom_user_read" << std::endl;
        return 0.0;
    }
}

float eeprom_user_subtotal_read(UserStruct data, uint8_t index) {
    return EEPROM.readInt(data.user_offset + 27 + index * 4);
}

float eeprom_user_read(UserStruct *data, uint8_t userIdx)
{
    for (uint8_t index = 0; index < UAH_AMOUNT; index++)
    {
        data[userIdx].uah_quantity[index] = EEPROM.readInt(data[userIdx].user_offset + 1 + index);
    }

    for (uint8_t index = 0; index < RUB_AMOUNT; index++)
    {
        data[userIdx].rub_quantity[index] = EEPROM.readInt(data[userIdx].user_offset + 7 + index);
    }

    for (uint8_t index = 0; index < USD_AMOUNT; index++)
    {
        data[userIdx].usd_quantity[index] = EEPROM.readInt(data[userIdx].user_offset + 13 + index);
    }

    for (uint8_t index = 0; index < EUR_AMOUNT; index++)
    {
        data[userIdx].eur_quantity[index] = EEPROM.readInt(data[userIdx].user_offset + 19 + index);
    }
}