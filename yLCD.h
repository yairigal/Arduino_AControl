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

void setupLCD(){
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  lcd.createChar(5, tempSign);

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
  String firstLine = "";
  firstLine += acState ? "On" : "Off";
  firstLine += "| ";
  firstLine += String(temperature);
  String secondLine = "----------------";
  if(numActions > 0){
    secondLine = String(actions[0].time.selectedDay);
    secondLine += "/";
    secondLine += String(actions[0].time.selectedMonth);
    secondLine += "-";
    if(actions[0].time.selectedHour < 10)
          secondLine += "0";
    secondLine += String(actions[0].time.selectedHour);
    secondLine += ":";
    if(actions[0].time.selectedMin < 10)
          secondLine += "0";
    secondLine += String(actions[0].time.selectedMin);
    secondLine += "-";
    secondLine += actions[0].on ? "On" : "Off";
  }
  lcd.clear();
  lcd.print(firstLine);
  lcd.write(5); // print temp sign
  lcd.print("C");
  lcd.print(" |");
  lcd.print(acMode == 2? "HOT":"COLD");
  lcd.setCursor(0,1);
  lcd.print(secondLine);
}

#endif
