#include "DisplayController.h"
#include <Button.h>
#include <Task.h>
#include "RandomTimeTask.h"
#include "SoundPlayer.h"
#include "TrainController.h"
#include "SettingsManager.h"

#define BUTTON_1 A1
#define BUTTON_2 A2
#define BUTTON_3 A3

#define LED_1 5
#define LED_2 6
#define LED_3 7

SettingsManager settingsManager;
TaskManager taskManager;
SoundPlayer soundPlayer;
TrainController trainController;
RandomTimeTask autoDriveTask(1000);
RandomTimeTask trainSoundTask(1000);
RandomTimeTask stationSoundTask(1000);

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

  settingsManager.setDisplayController(displayController.u8g2);

  soundPlayer.init();
  trainController.init();

  setCallbacks();  
  loadSettings();

  displayController.setStatusMessage(String("FOUND SOUND FILES: " + String(soundPlayer.getSongCount())), true);
}

void loop() {
  checkButtons();
  taskManager.Loop();
  
  trainController.handle();

  displayController.setAutoDriveTime(autoDriveTask.remainingTimeInSeconds());
  displayController.setTrainSoundTime(trainSoundTask.remainingTimeInSeconds());
  displayController.setStationSoundTime(stationSoundTask.remainingTimeInSeconds());
  displayController.setTrainStateAndLocation(trainController.getTrainState(), trainController.getTrainLocation());

  displayController.update();  
}

void autoDriveDepart()
{
  trainController.startTrain();
  if (trainController.getTrainLocation() == TrainLocationEast) {
    displayController.setStatusMessage(F("DEPART FROM STATION EAST"));
  } else {
    displayController.setStatusMessage(F("DEPART FROM STATION WEST"));
  }
  digitalWrite(LED_1, HIGH);
  delay(10);
  digitalWrite(LED_1, LOW);
}

void trainSoundPlay()
{
  int8_t num = trainController.playRandomSound();
  displayController.setStatusMessage(String("PLAYING TRAIN SOUND: " + String(num)));
  digitalWrite(LED_2, HIGH);
  delay(10);
  digitalWrite(LED_2, LOW);
}

void stationSoundPlay()
{
  int num = soundPlayer.playRandomSound();
  displayController.setStatusMessage(String("PLAYING STATION SOUND: " + String(num)));
  digitalWrite(LED_3, HIGH);
  delay(10);
  digitalWrite(LED_3, LOW);
}

void setCallbacks() {
  trainController.setArrivalCallback([](TrainLocation location) {
    if (location == TrainLocationEast) {
      displayController.setStatusMessage(F("ARRIVED AT STATION EAST"));
    } else {
      displayController.setStatusMessage(F("ARRIVED AT STATION WEST"));
    }
    if (autoDrive) taskManager.StartTask(&autoDriveTask);
  });


  autoDriveTask.setCallback(autoDriveDepart);
  trainSoundTask.setCallback(trainSoundPlay);
  stationSoundTask.setCallback(stationSoundPlay);
}

void loadSettings() {
  trainController.setTrainID(settingsManager.getSettingFor(SettingTrainID));
  trainController.setSpeed(settingsManager.getSettingFor(SettingTrainSpeed));
  
  soundPlayer.setVolume(settingsManager.getSettingFor(SettingVolume));
  
  autoDriveTask.setMinMaxTime((uint32_t) settingsManager.getSettingFor(SettingAutoDriveMinTime) * (uint32_t) 1000, (uint32_t) settingsManager.getSettingFor(SettingAutoDriveMaxTime) * (uint32_t) 1000);
  trainSoundTask.setMinMaxTime((uint32_t) settingsManager.getSettingFor(SettingTrainSoundMinTime) * (uint32_t) 1000, (uint32_t) settingsManager.getSettingFor(SettingTrainSoundMaxTime) * (uint32_t) 1000);
  stationSoundTask.setMinMaxTime((uint32_t) settingsManager.getSettingFor(SettingStationSoundMinTime) * (uint32_t) 1000, (uint32_t) settingsManager.getSettingFor(SettingStationSoundMaxTime) * (uint32_t) 1000);
}

void checkButtons() 
{
  button1.read();
  button2.read();
  button3.read();

  if (button1.pressedFor(3000)) {
    showSettingsLoadScreen();
    settingsManager.editAutoDriveSettings();
    loadSettings();
    return;
  }

  if (button2.pressedFor(3000)) {
    showSettingsLoadScreen();
    settingsManager.editTrainSoundSettings();
    loadSettings();
    return;
  }

  if (button3.pressedFor(3000)) {
    showSettingsLoadScreen();
    settingsManager.editStationSoundSettings();
    loadSettings();
    return;
  }

  if(button1.wasReleased()) {
    autoDrive = !autoDrive;
    displayController.setSchedulerState(AutomaticDriveScheduler, autoDrive);
    if (autoDrive) {
      autoDriveDepart();
      displayController.setStatusMessage(F("AUTO DRIVE ENABLED"));
    } else {
      taskManager.StopTask(&autoDriveTask);
      displayController.setStatusMessage(F("AUTO DRIVE DISABLED"));
    }
  }

  if(button2.wasReleased()) {
    trainSound = !trainSound;
    trainController.setSoundEnabled(trainSound);
    displayController.setSchedulerState(TrainSoundScheduler, trainSound);
    if (trainSound) {
      trainSoundPlay();
      taskManager.StartTask(&trainSoundTask);
      displayController.setStatusMessage(F("TRAIN SOUND ENABLED"));
    } else {
      taskManager.StopTask(&trainSoundTask);
      displayController.setStatusMessage(F("TRAIN SOUND DISABLED"));
    }
  }

  if(button3.wasReleased()) {
    stationSound = !stationSound;
    displayController.setSchedulerState(StationSoundScheduler, stationSound);
    if (stationSound) {
      stationSoundPlay();
      taskManager.StartTask(&stationSoundTask);
      displayController.setStatusMessage(F("STATION SOUND ENABLED"));
    } else {
      taskManager.StopTask(&stationSoundTask);
      displayController.setStatusMessage(F("STATION SOUND DISABLED"));
    }
  }
}

void showSettingsLoadScreen()
{
  wdt_reset();
  wdt_disable();

  autoDrive = false;
  displayController.setSchedulerState(AutomaticDriveScheduler, autoDrive);
  taskManager.StopTask(&autoDriveTask);

  trainSound = false;
  trainController.setSoundEnabled(trainSound);
  displayController.setSchedulerState(TrainSoundScheduler, trainSound);
  taskManager.StopTask(&trainSoundTask);

  stationSound = false;
  displayController.setSchedulerState(StationSoundScheduler, stationSound);
  taskManager.StopTask(&stationSoundTask);

  displayController.u8g2.clear();
  while(!digitalRead(BUTTON_1) || !digitalRead(BUTTON_2) || !digitalRead(BUTTON_3));
}