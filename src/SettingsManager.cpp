#include <Arduino.h>
#include <EEPROMex.h>
#include "SettingsManager.h"


SettingsManager::SettingsManager()
{
    EEPROM.setMaxAllowedWrites(100);
    if (EEPROM.read(0) != SETTINGS_VERSION) writeDefaults();
}

int8_t SettingsManager::getSettingFor(SettingIdentifier identifier)
{
    return EEPROM.read(identifier);
}

bool SettingsManager::setSettingFor(SettingIdentifier identifier, int8_t value)
{
    return EEPROM.update(identifier, value);
}

void SettingsManager::writeDefaults()
{
    EEPROM.write(0, SETTINGS_VERSION);

    EEPROM.write(SettingTrainID, DEFAULT_TRAIN_ID);
    EEPROM.write(SettingTrainSpeed, DEFAULT_TRAIN_SPEED);
    EEPROM.write(SettingVolume, DEFAULT_VOLUME);
    EEPROM.write(SettingAutoDriveMinTime, DEFAULT_AUTODRIVE_MINTIME);
    EEPROM.write(SettingAutoDriveMaxTime, DEFAULT_AUTODRIVE_MAXTIME);
    EEPROM.write(SettingTrainSoundMinTime, DEFAULT_TRAINSOUND_MINTIME);
    EEPROM.write(SettingTrainSoundMaxTime, DEFAULT_TRAINSOUND_MAXTIME);
    EEPROM.write(SettingStationSoundMinTime, DEFAULT_STATIONSOUND_MINTIME);
    EEPROM.write(SettingStationSoundMaxTime, DEFAULT_STATIONSOUND_MAXTIME);
}

void SettingsManager::editAutoDriveSettings()
{
    u8g2.setFont(/*u8g2_font_6x10_tr*/ u8g2_font_5x7_tr);
    u8g2.setFontRefHeightAll();

    uint8_t input;
    do {
        input = u8g2.userInterfaceSelectionList(String(F("AUTO DRIVE SETTINGS")).c_str(), 0, String(F("TRAIN ID\nTRAIN SPEED\nMIN DEPATURE TIME\nMAX DEPARTURE TIME\nSAVE & EXIT")).c_str());
        switch (input) {
            case 1:
                userInputForSetting(SettingTrainID, String(F("TRAIN ID")).c_str(), "ID ", 0, 255, 3, "");
                break;
            case 2:
                userInputForSetting(SettingTrainSpeed, String(F("TRAIN SPEED")).c_str(), "", 1, 127, 3, "");
                break;
            case 3:
                userInputForSetting(SettingAutoDriveMinTime, String(F("DEPATURE TIME")).c_str(), String(F("MIN ")).c_str(), 1, getSettingFor(SettingAutoDriveMaxTime), 3, " S");
                break;
            case 4:
                userInputForSetting(SettingAutoDriveMaxTime, String(F("DEPARTURE TIME")).c_str(), String(F("MAX ")).c_str(), getSettingFor(SettingAutoDriveMinTime), 255, 3, " S");
                break;
        };
    } while (input != 5);
}

void SettingsManager::editTrainSoundSettings()
{
    u8g2.setFont(/*u8g2_font_6x10_tr*/ u8g2_font_5x7_tr);
    u8g2.setFontRefHeightAll();

    uint8_t input;     
    do {
        input = u8g2.userInterfaceSelectionList(String(F("TRAIN SOUND SETTINGS")).c_str(), 0, String(F("MIN PAUSE TIME\nMAX PAUSE TIME\nSAVE & EXIT")).c_str());
        switch (input) {
            case 1:
                userInputForSetting(SettingTrainSoundMinTime, String(F("TRAIN SOUND TIME")).c_str(), String(F("MIN ")).c_str(), 1, getSettingFor(SettingTrainSoundMaxTime), 3, " S");
                break;
            case 2:
                userInputForSetting(SettingTrainSoundMaxTime, String(F("TRAIN SOUND TIME")).c_str(), String(F("MAX ")).c_str(), getSettingFor(SettingTrainSoundMinTime), 255, 3, " S");
                break;
        };
    } while (input != 3);

}
void SettingsManager::editStationSoundSettings()
{
    u8g2.setFont(/*u8g2_font_6x10_tr*/ u8g2_font_5x7_tr);
    u8g2.setFontRefHeightAll();

    uint8_t input;
    do {
        input = u8g2.userInterfaceSelectionList(String(F("STATION SOUND SETTINGS")).c_str(), 0, String(F("VOLUME\nMIN PAUSE TIME\nMAX PAUSE TIME\nSAVE & EXIT")).c_str());
        switch (input) {
            case 1:
                userInputForSetting(SettingVolume, String(F("VOLUME")).c_str(), "", 0, 30, 2, "");
                break;
            case 2:
                userInputForSetting(SettingStationSoundMinTime, String(F("STATION SOUND TIME")).c_str(), String(F("MIN ")).c_str(), 1, getSettingFor(SettingStationSoundMaxTime), 3, " S");
                break;
            case 3:
                userInputForSetting(SettingStationSoundMaxTime, String(F("STATION SOUND TIME")).c_str(), String(F("MAX ")).c_str(), getSettingFor(SettingStationSoundMinTime), 255, 3, " S");
                break;
        };
    } while (input != 4);
}

void SettingsManager::setDisplayController(U8G2 &displayController)
{
    u8g2 = displayController;
}

void SettingsManager::userInputForSetting(SettingIdentifier setting, const char *title, const char *pre, uint8_t lo, uint8_t hi, uint8_t digits, const char *post)
{
    uint8_t input = getSettingFor(setting);
    u8g2.userInterfaceInputValue(title, pre, &input, lo, hi, digits, post);
    setSettingFor(setting, input);
}
