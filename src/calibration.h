#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "main.h"
#include "EEPROMex.h"

// eeprom offset is 207 (after user)
void eeprom_calibration_init(uint16_t minSignal, uint64_t timedelta, int8_t currentCurrency, uint8_t index);
uint8_t eeprom_calibration_menu_type_read(uint8_t type);
uint8_t eeprom_calibration_last_currency_read();
uint16_t eeprom_calibration_signal_read(int8_t currentCurrency, uint8_t index);
uint64_t eeprom_calibration_time_read(int8_t currentCurrency, uint8_t index);

#endif
