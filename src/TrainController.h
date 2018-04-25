#include <Arduino.h>
#include <XpressNet.h>

#define XNetAddress 31    //Adresse im XpressNet
#define XNetSRPin 3       //Max485 Busdriver Send/Receive-PIN

#define TRAIN_ID 89
#define SOUND_DEPART 2
#define SOUND_ENGINE 1
#define SPEED 32

#define BUTTON_EAST 12
#define BUTTON_WEST 9

enum TrainLocation {
    TrainLocationEast,
    TrainLocationWest
};

enum TrainState {
    TrainStateDriving,
    TrainStateStopped
};

enum TrainSound : int8_t {
    TrainSoundSteamEngine = 1,
    TrainSoundWistle = 2,
    TrainSoundBell = 3,
    TrainSoundShortWistle = 4,
    TrainSoundShovel = 5,
    TrainSoundWistleEngineDriver = 7,
};

class TrainController {
    public: 
        typedef void (*TrainArrivedCallbackFunction)(TrainLocation location);

        TrainController();
        void init();
        void handle();
        void startTrain();
        void startTrain(TrainLocation destination);
        void stopTrain();

        void setArrivalCallback(TrainArrivedCallbackFunction callback);

        TrainState getTrainState();
        TrainLocation getTrainLocation();

        void setSoundEnabled(bool enabled);
        void startSound(TrainSound sound);
        void stopSound(TrainSound sound);
        void playSound(TrainSound sound);
        void stopAllSounds();
        int8_t playRandomSound();


    private:
        TrainSound _allSounds[6] = {
            TrainSoundSteamEngine,
            TrainSoundWistle,
            TrainSoundBell,
            TrainSoundShortWistle,
            TrainSoundShovel,
            TrainSoundWistleEngineDriver
        };

        TrainSound _randomSounds[4] = {
            TrainSoundBell,
            TrainSoundShortWistle,
            TrainSoundShovel,
            TrainSoundWistleEngineDriver
        };

        XpressNetClass XpressNet;
        TrainArrivedCallbackFunction _arrivalCallback;

        TrainLocation _trainLocation;
        TrainState _trainState;
        TrainSound _lastRandomSound;
        bool _soundEnabled;

        void checkButtons();
        TrainSound getRandomSound();
};
