#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WebServer.h>
ESP8266WebServer server(port);


void sendDataFromRAM(){
      // send data back to confirm
    String *newData = dataToStr();
    debug("Data = "+*newData);
    server.send(200, "text/plain", *newData);
    if(newData != NULL){
      delete newData;
      newData = NULL;
    }
    
}

void setupServer(){
    LCDwrite("STARTING SERVER");
    logln("Setting up server");
    if (MDNS.begin("esp8266")) {
      debug("MDNS responder started");
    }

    server.on("/",[](){
        debug("client on /");
        server.send(200, "text/plain","Hello");
    });
    
    server.on("/get",HTTP_GET, [](){
        debug("client on /get");
        sendDataFromRAM();
    });

    server.on("/set", HTTP_POST,[](){
        debug("client on /set");
        //backup acState, he will be overwritten in the strToData;
        debug("acState="+String(acState));
        int currentAcState = acState;
        debug("currentState="+String(currentAcState));
        String data = server.arg(0);
        strToData(data);
        // the loaded acState was the wantedState;
        debug("after strToData");
        int wantedAcState = acState;
        debug("wanted State="+String(wantedAcState));
        // restore the old acState;
        acState = currentAcState;
        //save the data to SD.
        debug("before Writing to SD");
        writeDataToSD();
        debug("before sending to AC");
        sendCodesTask(wantedAcState);
        debug("before sending back");
        // send back to the sender.
        sendDataFromRAM();
    });

    server.on("/update",HTTP_POST, [](){
        debug("client on /update");
        String mActionsText = server.arg(0);
        int n;
        debug("server update: actions args="+mActionsText);
        String * mActions = split(mActionsText,n,',');
        debug("server update: after split");
        strToActions(mActions,n);
        debug("server update: numActions="+String(numActions));
        forl(numActions){
          debug("actions["+String(i)+"].time="+actions[i].time.toString());
        }
        debug("server update: after strToActions");
        debug("server update: mActions="+String(*mActions));
        if(mActions != NULL){
           delete[] mActions; 
           mActions = NULL; 
        }

        writeDataToSD();
        debug("saving to sd");
        sendDataFromRAM();
        debug("sending back");  
        printAcDataToLCD();
    });

    server.on("/overwrite",HTTP_GET,[](){
      debug("client on /overwrite");
      String html = "state 1,0 :<input id=\"state\" name=\"acState\" type=\"number\"/><br/>fan 1,2,3,4-auto :<input id=\"fan\" name=\"fan\" type=\"number\"/><br/>temp :<input id=\"temp\" name=\"temp\" type=\"number\"/><br/>mode 1-cold 2-hot:<input id=\"mode\" name=\"mode 1-cold 2-hot\" type=\"number\"/><br/><button id=\"sendbutton\" name=\"send\" type=\"button\"> SEND</button><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script><script>$(\"#sendbutton\").click(()=>{let str=\"\"; str +=$(\"#state\").val(); str +=\" \"; str +=$(\"#fan\").val(); str +=\" \"; str +=$(\"#temp\").val(); str +=\" \"; str +=$(\"#mode\").val(); str +=\" \"; str +=\"0\"; $.post(\""+overwriteIP+"\",{\"1\":str}, function (data){$(\".result\").html(data);});});</script>";  
      server.send(200,"text/html",html);
    });
    
    server.on("/overwrite",HTTP_POST,[](){
      debug("client on /overwrite");
      String data = server.arg(0);
      debug("overwrite: args="+data);
      strToParameters(data);
      writeDataToSD();
      sendDataFromRAM();
      printAcDataToLCD();
    });
    
    server.begin();
}



#endif
