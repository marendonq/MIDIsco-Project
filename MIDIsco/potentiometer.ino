
/*

Manejo de los potenciometros y envio de su señal como
mesaje de de contron change 

*/


void sendPot(){

  int significativeChange = 3;
  for(int i = 0; i < numPot; i++){
    lecture[i] = map(analogRead(i),0,1023,0,127);
  }

  for(int i = 0; i < numPot; i++){
    int diference = abs(lecture[i] - lastLecture[i]);
    if(diference > significativeChange){

      byte controlerNumber = 30 + i;
      byte value = lecture[i];
      sendControlChange(currentChannel,controlerNumber, value);
      lastLecture[i] = lecture[i];
    
    }
  }
}