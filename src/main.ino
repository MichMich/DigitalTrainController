#include "DisplayController.h"
#include <Button.h>
#include <Task.h>
#include "RandomTimeTask.h"
#include "SoundPlayer.h"
#include "TrainController.h"

#define BUTTON_1 A1
#define BUTTON_2 A2
#define BUTTON_3 A3

#define LED_1 5
#define LED_2 6
#define LED_3 7

TaskManager taskManager;
SoundPlayer soundPlayer;
TrainController trainController;
RandomTimeTask autoDriveTask(MsToTaskTime(30000), MsToTaskTime(90000));
RandomTimeTask trainSoundTask(MsToTaskTime(10000), MsToTaskTime(30000));
RandomTimeTask stationSoundTask(MsToTaskTime(10000), MsToTaskTime(30000));

Button button1 = Button(A1, false, true, 25);
Button button2 = Button(A2, false, true, 25);
Button button3 = Button(A3, false, true, 25);

DisplayController displayController;

bool autoDrive = false;
bool trainSound = false;
bool stationSound = false;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  displayController.init();
  displayController.setStatusMessage(F("BOOTING"), true);

  soundPlayer.init();
  trainController.init();

  trainController.setArrivalCallback([](TrainLocation location) {
    if (location == TrainLocationEast) {
      displayController.setStatusMessage(F("ARRIVED AT STATION EAST"));
    } else {
      displayController.setStatusMessage(F("ARRIVED AT STATION WEST"));
    }
    if (autoDrive) taskManager.StartTask(&autoDriveTask);
  });

  soundPlayer.setVolume(10);
  displayController.setStatusMessage(String("FOUND SOUND FILES: " + String(soundPlayer.getSongCount())), true);
  
  autoDriveTask.setCallback([]() {
    trainController.startTrain();
    displayController.setStatusMessage(F("INITIATED AUTO DRIVE"));
    digitalWrite(LED_1, HIGH);
    delay(50);
    digitalWrite(LED_1, LOW);
    taskManager.StopTask(&autoDriveTask);
  });

  trainSoundTask.setCallback([]() {
    int8_t num = trainController.playRandomSound();
    displayController.setStatusMessage(String("PLAYING TRAIN SOUND: " + String(num)));
    digitalWrite(LED_2, HIGH);
    delay(50);
    digitalWrite(LED_2, LOW);

  });

  stationSoundTask.setCallback([]() {
    int num = soundPlayer.playRandomSound();
    displayController.setStatusMessage(String("PLAYING STATION SOUND: " + String(num)));
    digitalWrite(LED_3, HIGH);
    delay(50);
    digitalWrite(LED_3, LOW);

  });
  

  //displayController.setStatusMessage(F("SYSTEM READY!"));
  displayController.update();
}

void loop() {
  checkButtons();
  taskManager.Loop();
  trainController.handle();

  displayController.setAutoDriveTime(autoDriveTask.remainingTimeInSeconds());
  displayController.setTrainSoundTime(trainSoundTask.remainingTimeInSeconds());
  displayController.setStationSoundTime(stationSoundTask.remainingTimeInSeconds());

  displayController.update();  
}

void checkButtons() {
  button1.read();
  button2.read();
  button3.read();

  if(button1.wasPressed()) {
    autoDrive = !autoDrive;
    displayController.setSchedulerState(AutomaticDriveScheduler, autoDrive);
    if (autoDrive) {
      taskManager.StartTask(&autoDriveTask);
      displayController.setStatusMessage(F("AUTO DRIVE ENABLED"));
    } else {
      taskManager.StopTask(&autoDriveTask);
      displayController.setStatusMessage(F("AUTO DRIVE DISABLED"));
    }
  }

  if(button2.wasPressed()) {
    trainSound = !trainSound;
    trainController.setSoundEnabled(trainSound);
    displayController.setSchedulerState(TrainSoundScheduler, trainSound);
    if (trainSound) {
      taskManager.StartTask(&trainSoundTask);
      displayController.setStatusMessage(F("TRAIN SOUND ENABLED"));
    } else {
      taskManager.StopTask(&trainSoundTask);
      displayController.setStatusMessage(F("TRAIN SOUND DISABLED"));
    }
  }

  if(button3.wasPressed()) {
    stationSound = !stationSound;
    displayController.setSchedulerState(StationSoundScheduler, stationSound);
    if (stationSound) {
      taskManager.StartTask(&stationSoundTask);
      displayController.setStatusMessage(F("STATION SOUND ENABLED"));
    } else {
      taskManager.StopTask(&stationSoundTask);
      displayController.setStatusMessage(F("STATION SOUND DISABLED"));
    }
  }
}