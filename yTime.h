#ifndef TIME_H
#define TIME_H

#include <time.h> 

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
  time(nullptr);
}

void getTime(int &year,int &month,int &day,int& hour, int& minute){
  do{
    time_t now = time(NULL);
    struct tm * ptm = gmtime(&now);
    year = ptm->tm_year + 1900;
    month = ptm->tm_mon + 1;
    day = ptm->tm_mday;
    hour = ptm->tm_hour;
    minute = ptm->tm_min;
    debug("getTime: currentYear="+String(year));
  }while(year < 2018);
}



void checkTime(){
  Serial.print("checking for time ");
  //LCDwrite("CHECKING TIMES..");
  int year,month,day,hour,minute;
  getTime(year,month,day,hour,minute);
  DateTimeContainer current(year,month,day,hour,minute);
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
