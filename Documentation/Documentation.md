# MIDIsco's Documentation 

Hello,

First and foremost, thank you for using our software. We hope you find it enjoyable, useful, and easy to understand. Feel free to use this base code to explore the uncharted territories of your imagination and creativity. Remember, it can always be a "disco", but it's up to you to decide if it will truly become "MIDIsco."

Best regards,

__MIDIsco Team__

---
## Index

1. Hardware Model
2. Arduino's code
    * Scale and notes
    * Push buttons and potentiometers
    * Verification functions and additionals
    * Channels
    * Set up
    * Loop
3. Communication bridge
    * Raspberry PI Side
    * PC Side
    * Additional software  
        1. LoopMIDI 
        2. Hairless MIDI
        3. Ableton live

4. Extras

---
## 1. Hardware model

Important: This is a SAP project where we provide only the essential components to run a powerful MIDI. However, remember that you have the freedom to experiment and can always create the connections you need.

With that clear, we shall continue to the elements we will need to use to create MIDIsco:

- Raspberry PI 3 or higher
- SD memory 8GB or higher
- Cable Arduino-USB 
- Monitor (with USB cable)
- Keyboard (with USB cable)
- Mouse (with USB cable)
- Arduino Uno or higher (the Mega or Mega 2560 models are recommended)
- 16 push buttons **
- 2 potentiometers **
- 2 switches **

#### Additional Notes:

Elements marked with (**) will be connected to the Arduino, so if cables, resistors, or soldering are needed, you will need to procure those as well.

Then, after we have everything we need, we can start with the connections:

1. Connect the push buttons, potentiometers, and switches to their corresponding ports (analog or digital) on the Arduino board.

2. Connect the Arduino to the Raspberry PI using the Arduino-USB cable.

3. Finally, connect the previously mentioned peripherals to the Raspberry PI.

And with that we can say that our Hardware Model it's ready, now, it's time to code.

## 2. Arduino's code

Here are the steps for configuring, editing, and correctly executing the Arduino code.

### 2.1 Scales and Notes

>***const int numScales = 3***

This variable defines the number of scales used in the scales array, with each note represented in its MIDI format.

>***const int numNotes = 7***

This variable specifies the number of notes each scale will have. In this base code, we use the seven diatonic notes.

>***const byte scales[numScales][numNotes] = {{},{},{},...}***

This is the structure of the scales array.

>***int currentScale = 2***

This variable indicates the current scale and serves as a pivot to navigate between the available scales. It also ensures that a scale is always selected, preventing silences or delays.

### 2.2 Push Buttons and Potentiometers

#### 2.2.1 Push Buttons

>***const int numPush = 8***

This variable defines the number of push buttons connected to the Arduino.

>***const int numPushCC = 3***

This variable defines the number of control change push buttons for MIDI.

>***const int numPushCP = 1***

This variable defines the number of program change push buttons for MIDI.

Each button pin is created with the most compatible pins for each type of message.

>***const int pinPushNote[] = {23,24,25,26,27,28,29}***

>***const int pinPushCC[] = {36,35,34}***

>***const int pinPushCP[] = {37}***

#### 2.2.2 Potentiometer

>***const int numPot = 2;***

Here we define the number of potentiometers that we are going to use in your MIDIsco.

>***int lecture[numPot];***

this integer give us the value of the active Potentiometer, and it also help us making the action of "let the MIDI know that we have just change it value", but, it can do all by itself, for this script to run, we would also need a variable to compare, and there is where this code line makes sense:

>***int lastLecture[numPot];***  

it save and return the last state that has been registered in the potentiometer.

### 2.3 Verification functions and additionals

To ensure a rigorous tracking of message sending, a list of the last pressed notes is maintained. As soon as they are released, the respective Note Off messages are sent, stopping the note. These lines are represented in the code as follows:

>***int lastStatePushCC[numPushCC] = {0};***
>***int lastStatePushCP[numPushCP] = {0};***
>***int lastStatePush[numPush] = {0};***

Additionally, we have two more variables:

>***unsigned long lastLecture[numPush] = {0};***  
>***unsigned long intervalDebounce = 50;***

__lastLecture__ corresponds to the last reading made, and __intervalDebounce__ stipulates the minimum time a key must be pressed to sound. For the project, it is recommended to set this to 50, which represents 0.05 seconds in real time.

### 2.4 Channels

The folowing variable defines the number of channels used to send the information of the potentiometers and buttons. In the MIDI protocol, channels 0 to 15 are available, representing the 16 channels used for sending different sounds.

>***const int numChannels = 2;***  

Here, a list of channels to be specifically used is defined. Channel 0 (1) is used for sending piano notes or any instrument, and channel 9 (10) is specifically used for percussion sounds.

>***const int channels[] = {0, 9};***

This variable is used to specify which channel will be used at the beginning of the program. In this case, channel 0 is used.

>***int currentChannel = channels[0];***


### 2.5 Setup

In this section, it's important to mention the serial connection transmission speed. It is recommended to set it to **115200** baud, as this value has shown compatibility with various MIDI communication software.

>***Serial.begin(115200);***

After defining all the inputs the Arduino will receive, a for loop is used to obtain the initial value of each potentiometer and store them in the arrays **lecture** and **lastLecture**. This allows tracking changes in the potentiometers after initialization.

### 2.6 loop

The loop uses the functions defined for each type of message. 

First, let's recall that a MIDI message consists of three parts:

#### MIDI Message

1. **Status**: This part of the message indicates the type of message to be sent. In this project

    * The **NoteON** message (indicating a note with a specific velocity).

    * The **NoteOFF** message (indicating the end of a note).
    
    * The **Control Change** message indicating a change in control parameters within MIDI messages.
    
    It also indicates the channel to which the message will be sent.

   __Message format:__  
   
   >MMMMCCCC where (MMMM) is the message type and (CCCC) is the channel
   
   > An example would be 10010001 where 1001 is the *NoteON* message and 0001 is channel 2.

2. **data1**: This data can indicate the value of the note to be sent or the program change to be made (refer to the types of CC messages in the official MIDI documentation at [MIDI Association](www.midi.org)).

3. **data2**: This data sends values such as the velocity of the note or the value of the CC message.

This message format is used to send all the messages used within MIDIsco.

First, the potentiometers are read using the variables *lecture* and *lastLecture*.

> sendPot();

Then, messages of various types are sent:

> sendKeypad(scales[currentScale]);

Here, the defined buttons are read to send *NoteON* or *NoteOFF* messages, with the scale to be listened to provided as a parameter for each button press.

> sendKeypadCC();

Here, the defined buttons are read to send a *Control Change* message.

> changeChannel();


Here, the defined buttons are read for channel change or Program Change message.

###
## 3. Communication Bridge

After successfully transmitting the hexadecimal message from the Arduino, the next step is to capture it on the Raspberry Pi. Once captured, the message will be forwarded to the PC using the MQTT protocol.

We already know that the logic is implemented in our base code, but that doesn't mean the code is ready to run. Before it can be fully functional,we will have to be sure that we already satisfied some libraries requirements and we will need to modify some variables in order to adapt them to our personal Venv, Let’s go step by step:

### 3.1 Raspberry Pi Side

This part of the code can be found inside the "sideOfRasp.py" file.

The first step is to confirm that we can make the necessary imports. To do this, open a terminal on your **Raspberry Pi** and follow the next instructions:

#### Steps to Install Necessary Libraries for the Code to Run on a Raspberry Pi with Debian

Update the system and packages:

>***sudo apt update***  
***sudo apt upgrade***  

Install pip if it is not already installed:

>***sudo apt install python3-pip***  

Install the required libraries:

>***pip3 install pyserial paho-mqtt***  

By following these steps, you will install "pyserial" and "paho-mqtt", which are the necessary libraries for your script to function properly on your Raspberry Pi running Debian.

The next theme of concern if you want to fully adapt the code and run it without problems is the variables.

In the sideOfRasp.py file you will only have to edit this one:

>serial_port = '/dev/ttyACM0'

Adjust this variable based on your raspberry's serial port

If you don't know which port are you using on your raspberry you can always verify following this steps:

#### How to know which serial port am I using?

To determine which serial port your arduino is connected to on a Raspberry Pi, you can use several methods, but here is the method that we recommend to follow:

***Check the /dev Directory***  

List the serial devices before and after plugging in your device:

Open a terminal and run:

>***ls /dev/tty****  

Plug in your device and run the command again:

>***ls /dev/tty****   

Compare the output:

Before plugging in your device, you might see a list of serial devices, but, after plugging in your device, you should see additional entries, the additional entry that you see should be your "serial_port" variable.

with that configuration you should be able to send the Hexadecimal message to the MQTT broker, but now we have to catch that message from our PC side, so, let's see what we need.

### 3.2 PC side

This part of the code can be found inside the "sideOfPC.py" file.

The first step is to confirm that we can make the necessary imports. To do this, open a terminal on your **PC** and follow the next instructions:

#### Steps to Install Necessary Libraries for the Code to Run on a Windows PC 

**Note about pip:** You need to have pip to install the following libraries. Since this is a general check for most users and it's long enough to make this explanation look boring, we'll skip it for now. However, if you need that info anyway, you will find it at the end of the documentation in the "Extras".

In order to install the required libraries, you will have to open your terminal and use the following commands:

Install paho-mqtt library:
>***pip install paho-mqtt***  

Install mido library:

>***pip install mido***

By following these steps, you will install the necessary libraries for your script to function properly on your PC.

The next theme of concern if you want to fully adapt the code and run it without problems is the variables.

In the sideOfPC.py file, you will only have to edit this one:

>***loopmidi_port_name = 'MIDIport 2'***

This variable holds the name of the LoopMIDI port. 

If you are wondering how to know the name of your loopMIDI port because you haven't read this before and directly tried to put the name you saw in loopMIDI and the code didn't run, it's normal. The ports you create in loopMIDI and the internal representation of those ports are usually not the same. But don't worry, we have a solution for your problem. Inside the code, a control for this error has been implemented. If you check your terminal, you should see this message (if you haven't run your code, run it leaving a whitespace in that variable):

>*"Puertos MIDI disponibles:"*

If you are wondering how to know the name of your loopMIDI port because you haven't read this before and directly tried to put the name you saw in loopMIDI and the code didn't run, it's normal. The ports you create in loopMIDI and the internal representation of those ports are usually not the same. But don't worry, we have a solution for your problem. Inside the code, a control for this error has been implemented. If you check your terminal, you should see this message:

>**nameYouAssignedToYourPort (Number)**

Note: For example, when we created this code, we assigned the name "MIDIport" to the port in loopMIDI, but inside the PC, the real name of that port was "MIDIport 2".

Copy that exact name into the variable in question, and you will have the two sides of your MQTT connection working.

### 3.3 Additional Software

The additional software (and their download links) that we are going to use for this code are:

* [LoopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) follow this link and click on "download loopMIDI"

* [Hairless MIDI](https://projectgus.github.io/hairless-midiserial/) follow this link and scroll until you see "Dowloads" and click on the one that fit better to you 

* [Ableton live](https://www.ableton.com/es/trial/) again, choose the one that fit you better and download it. (feel free to use the trial or official version, but keep in mind the limitation of each one)

Once we have already install the additional software, we have to config each one, so, let's see how we are going to do it:

#### LoopMIDI configs

After successfully download and open our LoopMIDI, we will have to create two LoopMIDI's ports, one of them is going to be used as "reception" port, and the other is the one that is going to transfer the MIDI's message to ableton. if you make it right, you will end with a view like this:

 
![Relative](.\LoopMIDI_CorrectView.png)

Where "MIDIport" is going to be the port where we upload the MIDI message and "loopMIDI Port" is going to be the one who transfer the MIDI message to ableton, but, for now we are not able to clearly see that, so, let's move on to the next step, to clarify that.

#### Hairless MIDI configs

After successfully download and open our Hairless MIDI, we will have to find the "MIDI In" and "MIDI Out" ports and assign the two ports that we have just created in the **LoopMIDI configs**:
 
![Relative](.\HairlessMIDI_CorrectView.png)

If everything goes well until now you will get the exact same output as we shown in the example.

When we press any of the push buttons we should get this kind of output: 

![Relative](.\HairlessMIDI_CorrectOutputView.png)

and with that we are ready to finally connect the ableton software and hear our __WIFI MIDI__ singing

#### Ableton live configs

After successfully download and open our Ableton Live, we will have to find the "Analog" ports and after select it with one click, we have to drag it to the first slot how is shown in the following example:

![Relative](.\AbletonLive_CorrectView.png)

Now, enjoy <3






