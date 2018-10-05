#ifndef GLOBALS_H
#define GLOBALS_H


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
