
/*

Mensajes basicos del protocolo MIDI

*/


void MIDImessage(byte status, byte data1, byte data2){
/*
  // descomentar para seguir el flujo de los mensajes 
  Serial.print("Status: ");
  Serial.print(status, HEX);
  Serial.print(", Data1: ");
  Serial.print(data1, HEX);
  Serial.print(", Data2: ");
  Serial.println(data2, HEX);
*/


  Serial.write(status);
  Serial.write(data1);
  Serial.write(data2);

}


// Crear mediante la operacion el byte de status
byte createStatus(byte channel, byte message){
  byte status = (message << 4) | channel;
  return status;
}

// envia un mensaje de note on
void sendNoteOn(unsigned char channel, unsigned char data1, unsigned char data2){ 

  byte status = createStatus(channel,0b1001);

  MIDImessage(status, data1, data2);

}

// envia un mensaje de note off
void sendNoteOff(byte channel, byte data1, byte data2){

  byte status = createStatus(channel, 0b1000);

  MIDImessage(status, data1, data2);

}


// envia un mensaje de control change (para potenciometros y algunos botenes)
void sendControlChange(byte channel, byte controlerNumber, byte value){
  byte status = createStatus(channel,0b1011);
  MIDImessage(status,controlerNumber,value);
}

// envia un mensaje de cambio de programa 
void sendProgramChange(byte channel, byte programNumber){

  byte status = createStatus(channel,1100);
/*
  Serial.print("Status: ");
  Serial.print(status, HEX);
  Serial.print(", programNumber: ");
  Serial.println(programNumber, HEX);
*/

  Serial.write(status);
  Serial.write(programNumber);

}