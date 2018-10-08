#ifndef ACREMOTE_H
#define ACREMOTE_H

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <electraAcRemoteEncoder.h>

#define FREQ_HZ 38000
#define TIMINGS_LENGTH 200


char acCommand[256];
// Connect 5v to long leg of IR and this pin to short leg.
int outputPin = D2;


///////AC COMMANDS///////
void mark (int time)
{
    int i = 0;
    int cycles = 0;
    cycles = time / 28; /*The carrier wave is 38kHz so each period is 26 micro seconds. After checking the output with an IR receiver, 28 seemed to be better
                          as the hardware isn't as precise as you would hope */
    if (cycles == 0){
        cycles = 1;
    }
    for (i = 0;i < cycles;i++){
        digitalWrite(outputPin, HIGH);
        delayMicroseconds(13);
        digitalWrite(outputPin, LOW);
        delayMicroseconds(13);
    }
}

void space (int time)
{
    digitalWrite(outputPin, LOW);
    if (time > 0){
        delayMicroseconds(time);
    }
}
void irRemoteSendRaw (int *codes, int len)
{
    int i;

    for (i = 0;i < len;i++){
        if (i%2 == 0){
            mark(codes[i]);
        }
        else{
            space(codes[i]);
        }
    }
}
void sendCodesTask(int wantedState)
{
    struct airCon newAc;
    int *codes;
    int changeState;
    if(acState == wantedState)
      changeState = 1; // dont change state;
    else{
      changeState = 0;
      acState = 1 - acState;
    }
    if(temperature > 30){
      temperature = 30;
    }
    else if(temperature < 16){
      temperature = 16;
    }
    codes = getCodes(&newAc,fanStrength,acMode,temperature,changeState,acSwing);
    logln("Sending action to the AC");
    irRemoteSendRaw(codes,TIMINGS_LENGTH);
    lcd.clear();
    lcd.print("=====ACTION=====");
    delay(500);
    printAcDataToLCD();
}
//////////////////////////

#endif
