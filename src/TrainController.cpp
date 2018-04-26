#include "TrainController.h"

TrainController::TrainController()
{

}

void TrainController::init()
{
    pinMode(BUTTON_EAST, INPUT_PULLUP);
    pinMode(BUTTON_WEST, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    XpressNet.start(XNetAddress, XNetSRPin);    //Initialisierung XNet-Bus
}

void TrainController::handle()
{
    XpressNet.receive();
    checkButtons();
}

void TrainController::setTrainID(int8_t id)
{   
    _trainID = id;
}

void TrainController::setSpeed(int8_t speed)
{
    _speed = speed;
}


void TrainController::startTrain()
{
    startTrain((_trainLocation != TrainLocationEast) ? TrainLocationEast : TrainLocationWest);
}

void TrainController::startTrain(TrainLocation destination)
{
    if (_trainState == TrainStateDriving) return;

    playSound(TrainSoundWistle);
    
    byte speed = (destination == TrainLocationEast) ? _speed  : _speed + 128;
    XpressNet.setLocoDrive(0x00, _trainID, 0x13, speed);

    _trainState = TrainStateDriving;
}

void TrainController::stopTrain()
{
    TrainController::XpressNet.setLocoDrive(0x00, _trainID, 0x13, 0);

    _trainState = TrainStateStopped;
}

void TrainController::setArrivalCallback(TrainArrivedCallbackFunction callback)
{
    _arrivalCallback = callback;
}

TrainState TrainController::getTrainState()
{
    return _trainState;
}

TrainLocation TrainController::getTrainLocation()
{
    return _trainLocation;
}

void TrainController::setSoundEnabled(bool enabled)
{
    _soundEnabled = enabled;
    if (enabled) startSound(TrainSoundSteamEngine);
    if (!enabled) stopAllSounds();
}

void TrainController::startSound(TrainSound sound)
{
    if(_soundEnabled) XpressNet.setLocoFunc(0x00, _trainID, 1, sound);
}

void TrainController::stopSound(TrainSound sound)
{
    XpressNet.setLocoFunc(0x00, _trainID, 0, sound);
}

void TrainController::playSound(TrainSound sound)
{
    startSound(sound);
    stopSound(sound);
}

void TrainController::stopAllSounds()
{
    for( int8_t soundIndex = 0; soundIndex < sizeof(_allSounds); soundIndex++ ){
        TrainSound sound = _allSounds[soundIndex];
        stopSound(sound);
    }
}

int8_t TrainController::playRandomSound()
{
    TrainSound sound = getRandomSound();
    playSound(sound);
    return sound;
}

// PRIVATE

void TrainController::checkButtons()
{
  if (!digitalRead(BUTTON_EAST) && _trainLocation != TrainLocationEast) {
    _trainLocation = TrainLocationEast;
    stopTrain();
    _arrivalCallback(_trainLocation);
  }

  if (!digitalRead(BUTTON_WEST) && _trainLocation != TrainLocationWest) {
    _trainLocation = TrainLocationWest;
    stopTrain();
    _arrivalCallback(_trainLocation);
  }

}

TrainSound TrainController::getRandomSound()
{
    TrainSound newTrainSound;
    do {
        newTrainSound = _randomSounds[random(sizeof(_randomSounds))];
    } while(newTrainSound == _lastRandomSound);
    _lastRandomSound = newTrainSound;

    return newTrainSound;
}