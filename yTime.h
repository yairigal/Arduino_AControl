#ifndef TIME_H
#define TIME_H

#define MAX_SECONDS 2678400 // 30 days in seconds
#define seconds() (MyMillis()/1000)

#include <time.h> 
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "109.226.40.40", 3600 * 3, 0);


unsigned long bootupUnixTime = 0;
unsigned long secondsPassedFromBootup = 0;
unsigned long startupMillis = 0;

void deleteActions(int upToIndex){
  int newSize = numActions - upToIndex -1;
  if(upToIndex == numActions - 1){ // delete all actions
    if(actions != NULL){
      delete[] actions;
      actions = NULL;
    }
    numActions = 0;
    return;
  }
  
  FutureAction * newActions = &actions[upToIndex+1];
  
  for(int i=0;i<=upToIndex;i++){
      delete &actions[i];
  }

  actions = newActions;
  numActions = newSize;
}




unsigned long MyMillis(){
  return millis() - startupMillis;
}


DateTimeContainer getTime(){
  debug("getTime: seconds="+String(seconds()));
  if(seconds() > MAX_SECONDS){ // if overflow is about to occour
    bootupUnixTime += seconds(); // startupTime is set to current time;
    startupMillis = MyMillis(); // set startup millis as now so the difference is 0; 
  }
  time_t now = bootupUnixTime + seconds();
  struct tm * ptm = gmtime(&now);
  DateTimeContainer t;
  t.selectedYear = ptm->tm_year + 1900;
  t.selectedMonth = ptm->tm_mon + 1;
  t.selectedDay = ptm->tm_mday;
  t.selectedHour = ptm->tm_hour;
  t.selectedMin = ptm->tm_min;
  debug("getTime: currentTime="+t.toString());
  return t;
}

void initTime(){
  //configTime(3 * 3600, 0, "gr.pool.ntp.org", "de.pool.ntp.org","time.nist.gov");
  // il.pool.ntp.org - 109.226.40.40
  // gr.pool.ntp.org - 178.128.126.226, 195.167.30.249
  // time.nist.gov - 132.163.96.1
  //configTime(3 * 3600, 0, "109.226.40.40", "178.128.126.226","132.163.96.1");
  Serial.print("\nWaiting for time");
  LCDwrite("Waiting for time");
//  while (!time(nullptr)) {
//    Serial.print(".");
//    LCDwrite(".",true);
//    delay(1000);
//  }
  timeClient.begin();
  do{
    timeClient.forceUpdate();
    bootupUnixTime = timeClient.getEpochTime();
    current = getTime();
    debug("bootupUnixTime="+String(bootupUnixTime));
    debug("current Time="+current.toString());
  }while(current.selectedYear < 2018);
  
  timeClient.end();
  Serial.println("");

  //bootupUnixTime = time(nullptr);
  //debug("current Time="+String(time(0)));
  //getUnixTimeOnline();
}











void checkTime(){
  Serial.print("checking for time ");
  current = getTime();
  debug("current Time="+current.toString());
  // if its the time to do this.
  for(int i=numActions-1;i>=0;--i){
      DateTimeContainer topRecord = actions[i].time;
      debug("comparing times: now: "+current.toString()+" topRecord: "+topRecord.toString());
      if(current.after(topRecord) || current.equals(topRecord)){
        debug("comparing was succesful !");
        // do action
        Serial.println("\nfound time to on/off ac, executing...("+topRecord.toString()+")");
        //String onoff = actions[i].on ? "on" : "off";
        //LCDwrite("===TIME FOUND===");
        //LCDwrite("Turning "+onoff);
        sendCodesTask(actions[i].on?1:0);
        // remove from the the index to index 0;
        deleteActions(i);
        writeDataToSD();
        return;
      }
  }
  Serial.println("..done");
}




#endif
