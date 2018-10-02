#ifndef wifi_h
#define wifi_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

/////////////WIFI/////////////

//void settingUpStaticIP(){
//  // config static IP
//  IPAddress ip(192, 168, 1, 200); // where xx is the desired IP Address
//  IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
//  Serial.print(F("Setting static ip to : "));
//  Serial.println(ip);
//  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
//  WiFi.config(ip, gateway, subnet);
//}


bool reconnecting = false;

void connectToWiFi(){
  //settingUpStaticIP();
  // setted up the static ip in the router.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi");
  lcd.clear();
  lcd.print("Connecting WiFi");
  lcd.setCursor(0,1);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.print(".");
    delay(1000);
  }
  Serial.print("\nConnected to "+String(ssid)+"\nIP address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());
  delay(3000);
}


#endif
