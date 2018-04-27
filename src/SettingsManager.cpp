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
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontRefHeightAll();
    // uint8_t input = u8g2.userInterfaceSelectionList("AUTO DRIVE SETTINGS", 0, "TRAIN ID\nTRAIN SPEED\nMIN DEPATURE TIME\nMAX DEPARTURE TIME");
    // switch (input) {
    //     case 1:
    //         userInputForSetting(SettingTrainID, "TRAIN ID", "ID ", 0, 255, 3, "");
    //     case 2:
    //         userInputForSetting(SettingTrainSpeed, "TRAIN SPEED", "", 0, 127, 3, "");
    //     case 3:
    //         userInputForSetting(SettingAutoDriveMinTime, "DEPATURE TIME", "MIN ", 0, 255, 3, " S");
    //     case 4:
    //         userInputForSetting(SettingAutoDriveMaxTime, "DEPARTURE TIME", "MAX ", 0, 255, 3, " S");
    // }
}

void SettingsManager::editTrainSoundSettings()
{
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontRefHeightAll();
    // uint8_t input = u8g2.userInterfaceSelectionList("TRAIN SOUND SETTINGS", 0, "MIN PAUSE TIME\nMAX PAUSE TIME");
    // switch (input) {
    //     case 1:
    //         userInputForSetting(SettingTrainSoundMinTime, "TRAIN SOUND TIME", "MIN ", 0, 255, 3, " S");
    //     case 2:
    //         userInputForSetting(SettingTrainSoundMaxTime, "TRAIN SOUND TIME", "MAX ", 0, 255, 3, " S");
    // }
}
void SettingsManager::editStationSoundSettings()
{
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontRefHeightAll();
    // uint8_t input = u8g2.userInterfaceSelectionList("STATION SOUND SETTINGS", 0, "VOLUME\nMIN PAUSE TIME\nMAX PAUSE TIME");
    // switch (input) {
    //     case 1:
    //         userInputForSetting(SettingVolume, "VOLUME", "", 0, 30, 2, "");
    //     case 2:
    //         userInputForSetting(SettingStationSoundMinTime, "STATION SOUND TIME", "MIN ", 0, 255, 3, " S");
    //     case 3:
    //         userInputForSetting(SettingStationSoundMaxTime, "STATION SOUND TIME", "MAX ", 0, 255, 3, " S");
    // }
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
