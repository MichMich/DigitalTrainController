/*
  XpressNet for Arduino

  Showing the funktion of the Library for a Railpower OFF Button

  The circuit:
 * LED attached from pin 13 to ground and from pin 12 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * * Note: on most Arduinos there is already an LED on the board
     attached to pin 13.

*/


#include <XpressNet.h>
#include <elapsedMillis.h>

XpressNetClass XpressNet;

#define XNetAddress 31    //Adresse im XpressNet
#define XNetSRPin 3       //Max485 Busdriver Send/Receive-PIN

#define TRAIN_ID 89


// 1: Stoomgeluid (Altijd)
// 2: Fluit (Bij vertrek)
// 3: Bel (Af en toe)
// 4: Korte Fluit
// 5: Scheppen (Af en toe)
// 6: ??
// 7: Machinistfluit
//

#define SOUND_DEPART 2
#define SOUND_ENGINE 1

#define MIN_TIME 30000
#define MAX_TIME 60000
#define SPEED 32

#define BUTTON_START 6
#define BUTTON_END 7

bool direction = false;

bool departureScheduled = false;
int departureTime = 0;
elapsedMillis departureTimer;

//--------------------------------------------------------------------------------------------
// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(BUTTON_END, INPUT_PULLUP);
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  XpressNet.start(XNetAddress, XNetSRPin);    //Initialisierung XNet-Bus
  XpressNet.setLocoFunc(0x00,TRAIN_ID,1,SOUND_ENGINE);


  scheduleDeparture(false);
}

//--------------------------------------------------------------------------------------------
// the loop routine runs over and over again forever:
void loop() {
  XpressNet.receive();  //permernet update the library
  checkButton();  //check if Button push
  checkSchedule();
}

void scheduleDeparture(bool dir) {
  direction = dir;
  departureTimer = 0;
  departureScheduled = true;
  departureTime = random(MIN_TIME, MAX_TIME);
}
void checkSchedule() {
  if (departureTimer > departureTime && departureScheduled) {
    departureScheduled = false;
    depart(direction);
  }
}

void checkButton()
{
  if (!digitalRead(BUTTON_START)) {
    stop();
    scheduleDeparture(true);
  }

  if (!digitalRead(BUTTON_END)) {
    stop();
    scheduleDeparture(false);
  }

}

void stop() {
  XpressNet.setLocoDrive(0x00,TRAIN_ID,0x13,0);

}
void depart(bool direction) {
  digitalWrite(LED_BUILTIN, direction);

  byte speed = (direction) ? SPEED : SPEED+128;
  XpressNet.setLocoFunc(0x00,TRAIN_ID,1,SOUND_DEPART);
  XpressNet.setLocoFunc(0x00,TRAIN_ID,0,SOUND_DEPART);

  XpressNet.setLocoDrive(0x00,TRAIN_ID,0x13,speed);
}
