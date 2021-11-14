#include "sensor.h"
#include "calibration.h"
#include "menu.h"
#include "main.h"

extern Bounce rightBounce;


uint64_t Ir_first_time;
uint64_t Ir_second_time;
uint16_t maxSignal;
uint64_t timedelta;

uint16_t adcBuff[ADC_BUFF_LEN];


uint8_t adcBuffIdx = 0;
// extern int8_t currentCurrencyIdx;


enum coinState
{
    Idle,
    Ir_first,
    Analog,
    Ir_second
};

coinState state = Idle;

void sensorInit()
{
    //TODO attach speed interrupts
    ADCSRA = 0;            // clear ADCSRA register
    ADCSRB = 0;            // clear ADCSRB register
    ADMUX |= (0 & 0x07);   // set A0 analog input pin
    ADMUX |= (1 << REFS0); // set reference voltage
    ADMUX |= (1 << ADLAR); // left align ADC value to 8 bits from ADCH register

    // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
    // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
    //ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
    ADCSRA |= (1 << ADPS2); // 16 prescaler for 76.9 KHz
    //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

    ADCSRA |= (1 << ADATE); // enable auto trigger
    ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
    ADCSRA |= (1 << ADEN);  // enable ADC
    ADCSRA |= (1 << ADSC);  // start ADC measurements
}

void sensorTick()
{
    if (calibrationInProgress)
    {
    }
}

void onCoinTraveled(int8_t coinIndex)
{
}

void evaluateCoin(uint64_t timedelta, uint16_t maxSignal)
{
    int8_t coinIdx = -1;
    bool weightCorrect = false;

    for (uint8_t i = 0; i < currencyLen[currentCurrencyIdx]; i++)
    {
        if (abs(currencySignals[currentCurrencyIdx][i] - maxSignal) < ADC_CLEARENCE)
        {
            coinIdx = i;
            if (abs(currencyTimes[currentCurrencyIdx][i] - timedelta) < TIME_CLEARENCE)
            {
                weightCorrect = true;
            }
        }
    }

    coinTravledFlag = true;
    lastCoinIdx = coinIdx;
    lastWeightCorrect = weightCorrect;

    state = Idle;
}



void IR_first_interrupt()
{
    Ir_first_time = millis();
    state = Ir_first;
    maxSignal = 0;
}
void IR_second_interrupt()
{
    Ir_second_time = millis();
    state = Ir_second;
    timedelta = Ir_first_time - Ir_second_time;
    evaluateCoin(timedelta, maxSignal);
}

ISR(ADC_vect)
{
    adcBuff[adcBuffIdx] = ADCH << 8 | ADCL; // read 8 bit value from ADC
    adcBuffIdx++;
    uint16_t sum = 0;
    for (uint8_t i = 0; i < ADC_BUFF_LEN; i++)
    {
        sum += adcBuff[i];
    }
    uint16_t avg = sum / ADC_BUFF_LEN;

    if (avg > maxSignal)
    {
        maxSignal = avg;
    }
}

void calibration()
{
    calibrationInProgress = true;

    bool skippedCurrency[CURRENCY_AMOUNT] = {
        false,
        false,
        false,
        false,
    };

    for (uint8_t i = 0; i < currencyLen[currentCurrencyIdx]; i++)
    {
        menuCalibrationDisplayNominal(i, currentCurrencyIdx);
        while (true) //TODO or skip buton not pressed
        {
            rightBounce.update();
            if (rightBounce.fell())
            {
                skippedCurrency[i] = true;
                break;
            }
            if (coinTravledFlag)
            {
                coinTravledFlag = false;

                currencySignals[currentCurrencyIdx][i] = maxSignal;
                currencyTimes[currentCurrencyIdx][i] = timedelta;

                eeprom_calibration_init(maxSignal, timedelta, currentCurrencyIdx, i);

                break;
            }
        }
    }

    // clearEepromCalibrations();
    //TODO
    calibrationInProgress = false;
}

int numSamples = 0;
long t, t0;
