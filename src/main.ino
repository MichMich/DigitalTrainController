#include "DisplayController.h"
#include <Button.h>

#define BUTTON_1 A1
#define BUTTON_2 A2
#define BUTTON_3 A3


Button button1 = Button(A1, false, true, 25);
Button button2 = Button(A2, false, true, 25);
Button button3 = Button(A3, false, true, 25);

DisplayController displayController;

bool autoDrive = false;
bool trainSound = false;
bool stationSound = false;

void setup() {
  
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);

  displayController.init();

  displayController.setAutoDriveTime(0);
  displayController.setSoundEffectTime(0);
  displayController.setStatusMessage(F("BOOTING ..."));
  displayController.update(); 
  delay(1000);

  displayController.setStatusMessage(F("SYSTEM READY!"));
}

void loop() {
  button1.read();
  button2.read();
  button3.read();

  if(button1.wasPressed()) {
    autoDrive = !autoDrive;
    displayController.setSchedulerState(AutomaticDriveScheduler, autoDrive);
    displayController.setStatusMessage(autoDrive ? F("AUTO DRIVE ENABLED") : F("AUTO DRIVE DISABLED"));
  }

  if(button2.wasPressed()) {
    trainSound = !trainSound;
    displayController.setSchedulerState(TrainSoundScheduler, trainSound);
    displayController.setStatusMessage(trainSound ? F("TRAIN SOUNDS ENABLED") : F("TRAIN SOUNDS DISABLED"));
  }

  if(button3.wasPressed()) {
    stationSound = !stationSound;
    displayController.setSchedulerState(StationSoundScheduler, stationSound);
    displayController.setStatusMessage(stationSound ? F("STATION SOUNDS ENABLED") : F("STATION SOUNDS DISABLED"));
  }

  displayController.setAutoDriveTime(autoDrive ? millis() / 1000 : -1);
  displayController.setSoundEffectTime((trainSound || stationSound) ? millis() / 1000 : -1);

  displayController.update();  
}

