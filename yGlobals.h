#ifndef GLOBALS_H
#define GLOBALS_H

// SD
#include <SPI.h>
#include <SD.h>
// WIFI
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

File root;

DateTimeContainer current;
//////////AC mangmnet///////////
//TODO delete actions, words
FutureAction* actions; 
int numActions = 2;

////// AC GLOBALS //////
int acState = 0; // 1 - on, 0 - off
int fanStrength = 4; // 1,2,3,4 - auto
int temperature = 23; // 16 - 30
int acMode = 1; // 1 - cool , 2 - heat
int acSwing = 0; // not relevant for ours.

#endif
