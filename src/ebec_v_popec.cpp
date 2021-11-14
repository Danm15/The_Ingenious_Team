#include <Arduino.h>

#define coin_amount 6                                  
float coin_value[coin_amount] = {0.05, 0.25, 0.5, 1.0};
String currency = "UAH";                                

int coin_signal[coin_amount];             
int coin_quantity[coin_amount];          
byte empty_signal;                       
unsigned long standby_timer, reset_timer; 
float summ_money = 0;                   



#include <LiquidCrystal_I2C.h>
#include <EEPROMex.h>
#include <Bounce2.h>
#include <string.h>


LiquidCrystal_I2C lcd(0x27, 16, 2); 


boolean recogn_flag, sleep_flag = true;

#define button 2        
#define calibr_button 3 
#define IRsens A7      

int sens_signal, last_sens_signal;
boolean coin_flag = false;

void setup()
{
  Serial.begin(9600);
  delay(500);

  pinMode(button, INPUT_PULLUP);
  pinMode(calibr_button, INPUT_PULLUP);

  empty_signal = 1 + analogRead(IRsens);

  lcd.init();
  lcd.backlight();

  Serial.println(!digitalRead(calibr_button));
  if (!digitalRead(calibr_button))
  {
    Serial.println("cal");
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Clibration");
    delay(500);
    reset_timer = millis();
    while (1)
    {
      if (millis() - reset_timer > 3000)
      {

        Serial.println("eepr clr");
        for (byte i = 0; i < coin_amount; i++)
        {
          coin_quantity[i] = 0;
          EEPROM.writeInt(20 + i * 2, 0);
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("eeprom cleared");
        delay(100);
      }
      if (digitalRead(calibr_button))
      {
        Serial.println("cal b");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("cal begin");
        break;
      }
    }
    while (1)
    {
      for (byte i = 0; i < coin_amount; i++)
      {
        lcd.setCursor(0, 1);
        lcd.print(coin_value[i]);
        lcd.setCursor(13, 1);
        lcd.print(currency);
        last_sens_signal = empty_signal;
        while (1)
        {
          sens_signal = analogRead(IRsens);
          if (sens_signal > last_sens_signal)
            last_sens_signal = sens_signal;
          if (sens_signal - empty_signal > 3)
            coin_flag = true;
          if (coin_flag && (abs(sens_signal - empty_signal)) < 2)
          {
            coin_signal[i] = last_sens_signal;
            EEPROM.writeInt(i * 2, coin_signal[i]);
            coin_flag = false;
            break;
          }
        }
      }
      break;
    }
  }

  for (byte i = 0; i < coin_amount; i++)
  {
    coin_signal[i] = EEPROM.readInt(i * 2);
    coin_quantity[i] = EEPROM.readInt(20 + i * 2);
    summ_money += coin_quantity[i] * coin_value[i];
  }

  for (byte i = 0; i < coin_amount; i++)
  {
    Serial.println(coin_signal[i]);
  }

  lcd.print("TIT Piggy bank");
  lcd.setCursor(0, 1);
  lcd.print(summ_money);
}

bool mode = false;

void loop()
{
  if (!digitalRead(button))

  {

    lcd.clear();

    for (byte i = 0; i < coin_amount; i++)
    {
      lcd.setCursor(i * 3, 0);
      lcd.print((int)coin_value[i]);
      lcd.setCursor(i * 3, 1);
      lcd.print(coin_quantity[i]);
    }
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("TIT Piggy bank");
    lcd.setCursor(0, 1);
    lcd.print(summ_money);
  }

  last_sens_signal = empty_signal;
  while (1)
  {
    sens_signal = analogRead(IRsens);
    if (sens_signal > last_sens_signal)
      last_sens_signal = sens_signal;
    if (sens_signal - empty_signal > 3)
      coin_flag = true;
    if (coin_flag && (abs(sens_signal - empty_signal)) < 2)
    {
      recogn_flag = false;
      for (byte i = 0; i < coin_amount; i++)
      {
        int delta = abs(last_sens_signal - coin_signal[i]);
        if (delta < 30)
        {
          summ_money += coin_value[i];
          lcd.setCursor(0, 1);
          lcd.print(summ_money);
          coin_quantity[i]++;
          recogn_flag = true;
          break;
        }
      }
      coin_flag = false;
      if (!recogn_flag)
      {
        lcd.setCursor(0, 0);
        lcd.print("Incorrect coin");
        delay(100);
        lcd.setCursor(0, 0);
        lcd.print("TIT Piggy bank");
        lcd.setCursor(0, 1);
        lcd.print(summ_money);
      }
      standby_timer = millis();
    }
  }
}