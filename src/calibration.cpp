#include "calibration.h"

void eeprom_calibration_init(uint16_t minSignal, uint64_t timedelta, int8_t currentCurrency, uint8_t index) {
    // CURRENCY_SIGNALS_OFFSET 207
    EEPROM.writeInt(CURRENCY_SIGNALS_OFFSET + currentCurrency * 12 + index * 2, minSignal);
    // CURRENCY_TIMES_OFFSET 259
    EEPROM.writeInt(CURRENCY_TIMES_OFFSET + currentCurrency * 12 + index * 2, timedelta);
}

uint16_t eeprom_calibration_signal_read(int8_t currentCurrency, uint8_t index) {
    return EEPROM.readInt(CURRENCY_SIGNALS_OFFSET + currentCurrency * 12 + index * 2);
}

uint64_t eeprom_calibration_time_read(int8_t currentCurrency, uint8_t index) {
    return EEPROM.readInt(CURRENCY_TIMES_OFFSET + currentCurrency * 12 + index * 2);
}

uint8_t eeprom_calibration_last_currency_read() {
    return EEPROM.readInt(LAST_CURRENCY_OFFSET);
}

uint8_t eeprom_calibration_menu_type_read(uint8_t type) {
    switch (type) {
        case 0:
            return EEPROM.readInt(MENU_AMOUNT_OFFSET);
        case 1:
            return EEPROM.readInt(MENU_PERCENT_OFFSET);
        case 2:
            return EEPROM.readInt(MENU_WEIGHT_OFFSET);
        default:
            return -1;
    }
}