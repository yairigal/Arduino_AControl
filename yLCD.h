#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include "yGlobals.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte tempSign[] = {
    0b11100,
    0b11100,
    0b11100,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

byte dotSpace[] = {
  B00000,
  B00000,
  B00000,
  B01100,
  B01100,
  B00000,
  B00000,
  B00000
};

void setupLCD(){
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  lcd.createChar(5, tempSign);
  lcd.createChar(1, dotSpace);

  // Turn on the backlight.
  lcd.backlight();
}

void LCDwrite(String msg, bool apnd = false){
  if(!apnd)
    lcd.clear();
  else
    lcd.setCursor(0,1);
  lcd.print(msg);
}

void printAcDataToLCD(){
  lcd.clear();
  lcd.print(acState ? "On" : "Off");
  lcd.write(1); // spacer;
  lcd.print(String(temperature));
  lcd.write(5); // print temp sign
  //lcd.print("C");
  lcd.write(1); // spacer;
//  if(current.selectedHour < 10)
//        lcd.print("0");
  lcd.print(String(current.selectedHour));
  lcd.print(":");
  if(current.selectedMin < 10)
        lcd.print("0");
  lcd.print(String(current.selectedMin));
  lcd.write(1); // spacer;
  lcd.print(acMode == 2? "HO":"CO");
  lcd.setCursor(0,1);
  //second line
  if(numActions <= 0)
    lcd.print("----------------");
  else{
    lcd.print(String(actions[0].time.selectedDay));
    lcd.print("/");
    lcd.print(String(actions[0].time.selectedMonth));
    lcd.print("-");
    lcd.print(String(actions[0].time.selectedHour));
    lcd.print(":");
    if(actions[0].time.selectedMin < 10)
      lcd.print("0");
    lcd.print(String(actions[0].time.selectedMin));
    lcd.print("-");
    lcd.print(actions[0].on ? "On" : "Off");
  }
}

#endif
