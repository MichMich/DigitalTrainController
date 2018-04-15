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


// #include <XpressNet.h>
#include <elapsedMillis.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

// XpressNetClass XpressNet;
SoftwareSerial mySerial(10, 11);

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

#define BUTTON_1 A1
#define BUTTON_2 A2
#define BUTTON_3 A3

#define LED_1 7
#define LED_2 6
#define LED_3 5


bool direction = false;

bool departureScheduled = false;
int departureTime = 0;
elapsedMillis departureTimer;

 

//--------------------------------------------------------------------------------------------
// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);
  Serial.println("Start!");
  delay(1000);



  mySerial.begin (9600);
  mp3_set_serial (mySerial);
  mp3_set_debug_serial(Serial);
  mp3_set_device(2);

  // initialize the digital pin as an output.
  pinMode(BUTTON_END, INPUT_PULLUP);
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  

  // XpressNet.start(XNetAddress, XNetSRPin);    //Initialisierung XNet-Bus
  // XpressNet.setLocoFunc(0x00,TRAIN_ID,1,SOUND_ENGINE);


  // for (int i=0; i < foldersCount; i++) {
  //   // Query the total number of microSD card files
  //   mp3_get_folder_sum(i + 1);
  //   tracks[i] = mp3_wait_folder_sum();

  //   Serial.print("Find ");
  //   Serial.print(tracks[i]);
  //   Serial.print(" tracks in folder 0");
  //   Serial.print(i);
  //   Serial.println(".");
  // }

  mp3_set_volume(15);

  mp3_get_tf_sum ();
  int count = mp3_wait_tf_sum ();

  Serial.println("Found files: ");
  Serial.println(count);


  while (true) {
    int num = random(count) + 1;
    Serial.print("Play random: ");
    Serial.println(num);
    mp3_play(num);
    delayCountdown(10000);
  }

  scheduleDeparture(false);
}


void delayCountdown(int time) {
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  delay(time/4);

  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, LOW);
  delay(time/4);

  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  delay(time/4);

  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  delay(time/4);
}

//--------------------------------------------------------------------------------------------
// the loop routine runs over and over again forever:

unsigned long tick = 0;

void loop() {
  // XpressNet.receive();  //permernet update the library
  checkButton();  //check if Button push
  checkSchedule();

  tick++;
  // if (tick % 1500 == 0) digitalWrite(LED_1, !digitalRead(LED_1));
  // if (tick % 1500 == 500) digitalWrite(LED_2, !digitalRead(LED_2));
  // if (tick % 1500 == 1000) digitalWrite(LED_3, !digitalRead(LED_3));

  if (tick % 1500 == 0) {
    digitalWrite(LED_1, !digitalRead(LED_1));
    // Serial.println("Whoot!");
  }
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
  // XpressNet.setLocoDrive(0x00,TRAIN_ID,0x13,0);

}
void depart(bool direction) {
  digitalWrite(LED_BUILTIN, direction);

  byte speed = (direction) ? SPEED : SPEED+128;
  // XpressNet.setLocoFunc(0x00,TRAIN_ID,1,SOUND_DEPART);
  // XpressNet.setLocoFunc(0x00,TRAIN_ID,0,SOUND_DEPART);

  // XpressNet.setLocoDrive(0x00,TRAIN_ID,0x13,speed);
}
