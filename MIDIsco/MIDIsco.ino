/*
Este codigo esta hecho por: Mateo Amaya, Miguel Rendon
Implementacion del protocolo MIDI y sus funciones mas basicas
como lo son los mensajes de control, los mensajes de notas 
(Note on/Note off), cambio de programa, entre otros.

El fin es mostrar como se puede utilizar desarrollar de manera
sencilla una implementación de este protocolo.

*/

// Definicon de variables del programa

// Notas  
// Se pueden añadir escalas segun se desee
const int numScales = 3; // escalas que se desen agregar
const int numNotes = 7; // Numero de notas que tiene un escala 

/*
Numbers of percusion
42 Closed Hi Hat
38 Acoustic Snare
36 Bass Drum 1
65 High Timbale
40 Electric Snare
*/

const byte scales[numScales][numNotes] = {
  {60, 62, 64, 65, 67, 69, 71}, // Do mayor (C major)
  { 0x23, // Bombo 35
  0x24, // Bombo 36
  0x26, // Caja 38
  0x29, // Tom Bajo 41
  0x2A, // Hi-Hat Cerrado 42
  0x2E, // Hi-Hat Abierto 46
  0x30}, // Tom Alto 48}, // percusion
  {60, 63, 65, 66, 67, 70, 72}   // Do blues (C blues),
  // {62, 64, 66, 67, 69, 71, 73}, // Re mayor (D major)
  // {64, 66, 68, 69, 71, 73, 75}, // Mi mayor (E major)
  // {65, 67, 69, 70, 72, 74, 76}, // Fa mayor (F major)
};



int currentScale = 2;

// pulsadores
const int numPush = 8;
const int numPushCC = 3;
const int numPushCP = 1;
const int pinPushNote[] = {23,24,25,26,27,28,29};
const int pinPushCC[] = {36,35,34};
const int pinPushCP[] = {37};

// listas de estado y validacion
int lastStatePushCC[numPushCC] = {0};
int lastStatePushCP[numPushCP] = {0};
unsigned long lastPushLecture[numPush] = {0};
int lastStatePush[numPush] = {0};
unsigned long intervalDebounce = 50; // Intervalo de debouncing en milisegundos

int lastStatePlay = 0;
  
// potenciometros
const int numPot = 2;
int lecture[numPot];
int lastLecture[numPot]; // Variable para ver el valor lasterior registrado en el potenciometro


// channels
const int numChannels = 2; 
const int channels[] = {0,9}; // Añadir los canales que se quieran utilizar

int currentChannel = channels[0]; // Select currente channel


void setup() {
  Serial.begin(115200);

  for (int i : pinPushNote){
    pinMode(i, INPUT);
  }

  for (int i : pinPushCC){
    pinMode(i,INPUT);
  }

  for (int i: pinPushCP){
    pinMode(i,INPUT);
  }

  for (int i = 0; i < numPot; i++) {
    lecture[i] = map(analogRead(i), 0, 1023, 0, 127);
    lastLecture[i] = lecture[i]; 
  }

}

void loop() {

  sendPot();
  sendKeypad(scales[currentScale]);
  sendKeypadCC();
  changeChannel();


}
