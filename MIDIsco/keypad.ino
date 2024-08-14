

void sendKeypad(const byte* notes){

  // numPush - 1 por que son 7 notas y tenemos 8 botones
  for (int i = 0; i < numPush - 1; i++ ){
    
    unsigned long now = millis(); 
    
    int lecturePush = digitalRead(pinPushNote[i]);

    if ((now - lastLecture[i]) > intervalDebounce){
    
      if (lecturePush != lastStatePush[i]){
        lastStatePush[i] = lecturePush;

        if (lastStatePush[i] == 1){
          sendNoteOn(currentChannel, notes[i],127);
        
        }else{
          sendNoteOff(currentChannel,notes[i],0);
        }
      }
      lastPushLecture[i] = now;

    }

  }
}


void sendKeypadCC(){
  for (int i = 0; i < numPushCC; i++){
    
    int lecturePush = digitalRead(pinPushCC[i]);
    
    if (lecturePush != lastStatePushCC[i]){
      
      if(lecturePush == 1){
        sendControlChange(currentChannel,85 + i, 127);
      }else{
        sendControlChange(currentChannel, 85 + i, 0);
      }

      lastStatePushCC[i] = lecturePush;
    } 
  }
}


void changeChannel(){
  for (int i = 0; i < numPushCP; i++){
    int lecturePush = digitalRead(pinPushCP[i]);

    if (lecturePush != lastStatePushCP[i]){
      
      if(lecturePush == 1){
        currentChannel = channels[1];
        currentScale = 1;
      }else{
        currentChannel = channels[0];
        currentScale = 2;
      }
      lastStatePushCP[i] = lecturePush;
    } 
  }
}

