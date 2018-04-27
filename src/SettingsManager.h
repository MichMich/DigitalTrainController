#include <Arduino.h>
#include <U8g2lib.h>

#define SETTINGS_VERSION 1 // Changing this value will cause the Arduino to reset the EEPROM values.

#define DEFAULT_TRAIN_ID 89
#define DEFAULT_TRAIN_SPEED 32
#define DEFAULT_VOLUME 15
#define DEFAULT_AUTODRIVE_MINTIME 30
#define DEFAULT_AUTODRIVE_MAXTIME 90
#define DEFAULT_TRAINSOUND_MINTIME 10
#define DEFAULT_TRAINSOUND_MAXTIME 60
#define DEFAULT_STATIONSOUND_MINTIME 10
#define DEFAULT_STATIONSOUND_MAXTIME 60

enum SettingIdentifier : int {
    SettingTrainID = 1,
    SettingTrainSpeed = 2,
    SettingVolume = 3,
    SettingAutoDriveMinTime = 4,
    SettingAutoDriveMaxTime = 5,
    SettingTrainSoundMinTime = 6,
    SettingTrainSoundMaxTime = 7,
    SettingStationSoundMinTime = 8,
    SettingStationSoundMaxTime = 9
};

class SettingsManager {
    public:
        SettingsManager();
        int8_t getSettingFor(SettingIdentifier identifier);
        bool setSettingFor(SettingIdentifier identifier, int8_t value);
        void writeDefaults();
        void editAutoDriveSettings();
        void editTrainSoundSettings();
        void editStationSoundSettings();
        void setDisplayController(U8G2 &displayController);

    private:
        void userInputForSetting(SettingIdentifier setting, const char *title, const char *pre, uint8_t lo, uint8_t hi, uint8_t digits, const char *post);
        U8G2 u8g2;
};
