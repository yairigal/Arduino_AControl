#ifndef wifi_h
#define wifi_h



/////////////WIFI/////////////

//void settingUpStaticIP(){
//  // config static IP
//  IPAddress ip(192, 168, 1, 200); // where xx is the desired IP Address
//  IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
//  log(F("Setting static ip to : "));
//  logln(ip);
//  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
//  WiFi.config(ip, gateway, subnet);
//}


  


void connectToWiFi(){
  //settingUpStaticIP();
  // setted up the static ip in the router.
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, password);
  log("\nConnecting to WiFi");
  lcd.clear();
  lcd.print("Connecting WiFi");
  lcd.setCursor(0,1);
  while (WiFi.status() != WL_CONNECTED) {
    log(".");
    lcd.print(".");
    delay(1000);
  }
  log("\nConnected to "+String(ssid)+"\nIP address: ");
  logln(WiFi.localIP());
  lcd.clear();
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());
  delay(3000);
}

//void onDisconnected(const WiFiEventStationModeDisconnected& event){
//  logln("Wifi disconnected");
//  lcd.clear();
//  lcd.print("lost connection");
//  connectToWiFi();
//}



#endif
