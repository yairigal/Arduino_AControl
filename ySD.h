#ifndef SD_H
#define SD_H

#include <SPI.h>
#include <SD.h>

/////////////////SD/////////
const int chipSelect = D10; // use D0 for Wemos D1 Mini
File root;

String* dataToStr(){   
    String * str = new String();
    *str += String(acState);
    *str += " ";
    *str += String(fanStrength);
    *str += ' ';
    *str += String(temperature);
    *str += ' ';
    *str += String(acMode);
    *str += ' ';
    *str += String(acSwing);

    for(int i=0;i<numActions;++i){
      *str += ',';
      *str += actions[i].on ? "true" : "false";
      *str += ' ';
      *str += String(actions[i].time.selectedYear);
      *str += ' ';
      *str += String(actions[i].time.selectedMonth);
      *str += ' ';
      *str += String(actions[i].time.selectedDay);
      *str += ' ';
      *str += String(actions[i].time.selectedHour);
      *str += ' ';
      *str += String(actions[i].time.selectedMin);
    }
    return str;
}



int countLines(String contents,char delim='\n'){
  int count = 0;
  for(int i=0;i<contents.length();++i)
    if(contents[i] == delim)
      ++count;
  return count;
}

//need to delete after done using the return value
String* split(String text,int &arrSize,char delim = ' '){
  int fileSize = text.length();
  int numWords = countLines(text,delim)+1; // count words
  String* words = new String[numWords];
  int wordsIndex = 0;
  int lastIndex = 0;
  for(int i=0;i<fileSize;++i){
    if(text[i] == delim){
      words[wordsIndex++] = text.substring(lastIndex,i);
      lastIndex = i+1;
    }
  }
  words[wordsIndex++] = text.substring(lastIndex,fileSize);
  arrSize = numWords;
  return words;  
}

void strToActions(String * acts,int n){
  if(actions != NULL){
     delete[] actions;
     actions = NULL; 
  }
  debug("strToActions: n="+String(n));
  debug("strToActions: acts[0]="+acts[0]);
  numActions = n;
  if(n > 0){
    if(acts[0].equals("")){
      // empty string
      numActions = 0;
      return;
    }
    actions = new FutureAction[numActions];
    int numWords;
    String * words;
    for(int i=0;i<numActions;++i){
      words = split(acts[i],numWords);
      actions[i].on = words[0].equals("true") ? true : false;
      actions[i].time.selectedYear = words[1].toInt();
      actions[i].time.selectedMonth = words[2].toInt();
      actions[i].time.selectedDay = words[3].toInt();
      actions[i].time.selectedHour = words[4].toInt();
      actions[i].time.selectedMin = words[5].toInt();
      if(words != NULL){
          delete[] words; 
          words = NULL;
      }
    }
  }
    debug("strToActions: done");
}

void clearFile(char * file){
  root = SD.open(file,O_TRUNC|O_WRONLY);
  root.close();
}

void writeDataToSD(){
  //first line is the data: state, fan, temp, mode, swing
  //second line and on is the times:on, year,month,day,hour,min
  clearFile("data");
  root = SD.open("data",FILE_WRITE);
  root.seek(0);
  String * str = dataToStr();
  debug("writeDataToSD: str="+*str);
  int n = str->length()+1;
  char * buffer = new char[n];
  str->toCharArray(buffer,n);
  buffer[n-1] = '\0';
  debug("writeDataToSD: buffer="+String(buffer));
  root.print(buffer);
  if(str != NULL){
     delete str;
     str = NULL; 
  }
  if(buffer != NULL){
     delete buffer;
     buffer = NULL; 
  }
  root.close();
}

void strToParameters(String str){
    int numWords;
    String * words = split(str,numWords);
    if(numWords < 5){
      LCDwrite("SD READ ERROR");
      Serial.println("failed to read ac instance data");
      return;
    }
    acState = words[0].toInt();
    fanStrength = words[1].toInt();
    temperature = words[2].toInt();
    acMode = words[3].toInt();
    acSwing = words[4].toInt();
    if(words != NULL){
      delete[] words;
      words = NULL; 
    } 
}

void strToData(String buffer){
    int numLines;
    String * lines = split(buffer,numLines,',');
    if (numLines < 1){
      LCDwrite("SD ERROR");
      Serial.println("failed to read Data from sd");
      return;
    }
    //parse first line;
    strToParameters(lines[0]);
    //parse other lines
    if(numLines > 1){
      strToActions(lines+1,numLines-1);
    }else{
      numActions = 0;
      if(actions != NULL){
        delete[] actions;
        actions = NULL;
      }
    }
    if(lines != NULL){
      delete[] lines;
      lines = NULL;
    }
}


void readDataFromSD(){
    LCDwrite("LOADING DATA....");
    Serial.println("Loading Data from SD card");
    if(!SD.exists("data")){
      debug("no file found");
      SD.open("data",O_CREAT).close();
      debug("file created");
    }
    debug("file found, opening...");
    root = SD.open("data",FILE_READ);
    debug("file opened");
    root.seek(0);
    String str;
    while(root.available())
      str += (char)root.read(); 
    root.close();
    debug("readDataFromSD: done reading file");
    strToData(str);
    debug("readDataFromSD: done converting to data");

    acState = 0; // always off when came back from restart.
}

bool initSDCard(){
  LCDwrite("INIT SD CARD....");
  Serial.println("\r\nWaiting for SD card to initialize...");
  if (!SD.begin(chipSelect)) { // CS is D8 in this example
    Serial.print("sd card init failed!");
    LCDwrite("INIT FAILED",true);
    return false;
  }
  LCDwrite("INIT COMPLETED");
  Serial.println("sd card init completed");
  return true;
}


#endif
