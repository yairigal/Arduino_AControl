#ifndef DEBUG_H
#define DEBUG_H

#include <string>

#define DEBUG_FLAG false
#if DEBUG_FLAG
#define debug(x) Serial.println(x);
#else
#define debug(x)
#endif


////////// DEBUG ///////////
//void debug(String str){
//  if(DEBUG_FLAG)
//    Serial.println(str);
//}
//
//void debug(char * str){
//  debug(String(str));
//}
////////////////////////////


#endif
