#define forl(x) for(int i=0; i<x ; ++i)


#ifndef UNIT_TEST
#include <Arduino.h>
#endif

#include "yHidden.h";
#include "DateTimeContainer.h"
#include "yGlobals.h"
#include "yDebug.h"
#include "yLCD.h"
#include "yAcRemote.h"
#include "ySD.h"
#include "yWifi.h"
#include "yServer.h"
#include "yTime.h"


///////////TIME MANAGMENT/////////////
unsigned long prevMillis = 0,currentMillis;
//////////////////////////////////////

void setup() {
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  pinMode(outputPin,OUTPUT);
  setupLCD();
  if(!initSDCard())
    return;
  connectToWiFi();
  initTime();
  readDataFromSD();
  setupServer();
  Serial.println("Started Running!");
  LCDwrite("Started Running!");
  delay(1000);
  printAcDataToLCD();
}


void loop() {
  currentMillis = millis();
  if(currentMillis - prevMillis > 60000){ // 1 min
      debug("1 min passed entering checkTime()");
      prevMillis = currentMillis;
      //check time and apply actions.
      checkTime();  
      printAcDataToLCD();
  }
  if(WiFi.status() != WL_CONNECTED){
    if(!reconnecting){
      reconnecting = true;
      Serial.println("WiFi disconnected, reconnecting...");
      lcd.clear();
      lcd.print("reconnecting to");
      lcd.setCursor(4,1);
      lcd.print("WiFi");
    }
    WiFi.begin(ssid, password);
  }
  else {
    if(reconnecting)
      reconnecting = false;
    server.handleClient();
  }

}
