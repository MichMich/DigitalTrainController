#include "DisplayController.h"

DisplayController::DisplayController() : u8g2(U8G2_R0)
{

}

void DisplayController::init()
{
    u8g2.begin();
}

void DisplayController::update()
{
    if (_needsUpdate) {
        u8g2.firstPage();
        do {
            drawAutoDriveTimer();
            drawSoundTimer();
            drawSchedulerStates();
            drawStatus();
        } while ( u8g2.nextPage() );
        _needsUpdate = false;
    }
}

void DisplayController::setSchedulerState(Scheduler scheduler, boolean state)
{
    if (state != _schedulerSates[scheduler]) {
        _needsUpdate = true;
        _schedulerSates[scheduler] = state;
    }
}

void DisplayController::setAutoDriveTime(int time)
{
    if (_autoDriveTime != time) {
        _needsUpdate = true;
        _autoDriveTime = time;
    }
}

void DisplayController::setSoundEffectTime(int time)
{
    if (_soundEffectTime != time) {
        _needsUpdate = true;
        _soundEffectTime = time;
    }
}

void DisplayController::setStatusMessage(String message)
{
    _statusMessage = message;
    _needsUpdate = true;
}

// Private

void DisplayController:: drawAutoDriveTimer() {
    int time = (_schedulerSates[AutomaticDriveScheduler]) ? _autoDriveTime : -1;
    drawTimer(F("AUTO"), F("DRIVE"), time, 29);
}

void DisplayController:: drawSoundTimer() {
    int time = (_schedulerSates[TrainSoundScheduler] || _schedulerSates[StationSoundScheduler]) ? _soundEffectTime : -1;
    drawTimer(F("SOUND"), F("EFFECT"), time, 49);
}

void DisplayController::drawSchedulerStates() {
    u8g2.setDrawColor(1);

    if(_schedulerSates[AutomaticDriveScheduler]) {
        u8g2.drawBox(0, 0, 42, 10);
    } else {
        u8g2.drawFrame(0, 0, 42, 10);
    }

    if(_schedulerSates[TrainSoundScheduler]) {
        u8g2.drawBox(43, 0, 42, 10);
    } else {
        u8g2.drawFrame(43, 0, 42, 10);
    }

    if(_schedulerSates[StationSoundScheduler]) {
        u8g2.drawBox(86, 0, 42, 10);
    } else {
        u8g2.drawFrame(86, 0, 42, 10);
    }

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);
    u8g2.drawStr(4,8, "A-DRIVE");
    u8g2.drawStr(46,8,"TRN-SND");
    u8g2.drawStr(90,8,"STN-SND");
}

void DisplayController::drawStatus() {
    u8g2.drawHLine(0, 54, 128);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2_uint_t width  = u8g2.getStrWidth(_statusMessage.c_str());
    u8g2.drawStr(64 - width / 2, 63, _statusMessage.c_str());
}

void DisplayController::drawTimer(String topLabel, String bottomLabel, int timeInSeconds, int8_t yPositionBaseLine) {
    String time;
    if (timeInSeconds == -1) {
        time = F("-");
    } else if (timeInSeconds == 0) {
        time = F("NOW");
    } else {
        char buffer[10];
        int seconds = timeInSeconds % 60;
        int minutes = timeInSeconds / 60;
        sprintf(buffer, "%02d:%02d", minutes, seconds);
        time = buffer;
    }

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);

    u8g2.drawStr(0, yPositionBaseLine - 8, topLabel.c_str());
    u8g2.drawStr(0, yPositionBaseLine, bottomLabel.c_str());

    u8g2.setFont(u8g2_font_crox4hb_tr);

    u8g2_uint_t width  = u8g2.getStrWidth(time.c_str());
    u8g2.drawStr(128-width, yPositionBaseLine, time.c_str());
}