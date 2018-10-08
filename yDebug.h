#ifndef DEBUG_H
#define DEBUG_H

#include <string>


String logfile = "log.txt";





void logln(IPAddress msg){
  Serial.println(msg);
  root = SD.open(logfile,FILE_WRITE);
    root.print(current.toString() + " - ");
  root.print(msg);
  root.print('\n');
  root.close();
}

void logln(String msg){
  Serial.println(msg);
  root = SD.open(logfile,FILE_WRITE);
    root.print(current.toString() + " - ");
  root.print(msg);
  root.print('\n');
  root.close();
}



void log(String msg){
  Serial.println(msg);
  root = SD.open(logfile,FILE_WRITE);
  root.print(current.toString() + " - ");
  root.print(msg);
  root.close();
}

#define DEBUG_FLAG true
#if DEBUG_FLAG
#define debug(x) logln(x)
#else
#define debug(x)
#endif


void initlog(){
  if(!SD.exists(logfile)){
    SD.open(logfile,O_CREAT).close();
    debug("file created");
  }
}

////////// DEBUG ///////////
//void debug(String str){
//  if(DEBUG_FLAG)
//    logln(str);
//}
//
//void debug(char * str){
//  debug(String(str));
//}
////////////////////////////


#endif
