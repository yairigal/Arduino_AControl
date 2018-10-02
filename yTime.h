#ifndef TIME_H
#define TIME_H

#define MAX_SECONDS 2678400 // 30 days in seconds
#define seconds() (MyMillis()/1000)

#include <time.h> 

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

void initTime(){
  //configTime(3 * 3600, 0, "gr.pool.ntp.org", "de.pool.ntp.org","time.nist.gov");
  // il.pool.ntp.org - 109.226.40.40
  // gr.pool.ntp.org - 178.128.126.226, 195.167.30.249
  // time.nist.gov - 132.163.96.1
  configTime(3 * 3600, 0, "109.226.40.40", "178.128.126.226","132.163.96.1");
  Serial.print("\nWaiting for time");
  LCDwrite("Waiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    LCDwrite(".",true);
    delay(1000);
  }
  Serial.println(""); 
  bootupUnixTime = time(nullptr);
}

unsigned long MyMillis(){
  return millis() - startupMillis;
}



//DateTimeContainer getUnixTimeOnline(){
//    DateTimeContainer t;
//    time_t now;
//    struct tm * ptm;
//  do{
//    now = time(NULL);
//    ptm = gmtime(&now);
//    t.selectedYear = ptm->tm_year + 1900;
//    t.selectedMonth = ptm->tm_mon + 1;
//    t.selectedDay = ptm->tm_mday;
//    t.selectedHour = ptm->tm_hour;
//    t.selectedMin = ptm->tm_min;
//    debug("getTime: currentYear="+String(t.selectedYear));
//  }while(t.selectedYear < 2018);
//  bootupUnixTime = now;
//  return t;
//}

DateTimeContainer getTime(){
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
  return t;
}



void checkTime(){
  Serial.print("checking for time ");
  DateTimeContainer current = getTime();
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
