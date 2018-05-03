#include "DisplayController.h"

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

DisplayController::DisplayController() : u8g2(U8G2_R2)
{

}

void DisplayController::init()
{
    u8g2.begin(A1, A3, A2);
}

void DisplayController::update()
{
    if (_resetStatusTimer > STATUS_RESET_DELAY && _statusMessage != F("MODELTREINBAAN.NL")) {
        setStatusMessage(F("MODELTREINBAAN.NL"));
        // setStatusMessage(String(String(freeMemory()) + " BYTES FREE (RAM)" ));
    }

    if (_blinkTimer > BLINK_SPEED) {
        _blinkTimer = 0;
        _blink = !_blink;
        _needsUpdate = true;
    }

    if (_needsUpdate) {
        u8g2.firstPage();
        do {
            u8g2.setDrawColor(2);
            drawAutoDriveTimer();
            drawTrainSoundTimer();
            drawStationSoundTimer();
            drawSchedulerStates();
            drawStatus();
            if (_trainState == TrainStateDriving) drawDriveIndicator(67, 17, 57, 8);
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

void DisplayController::setTrainSoundTime(int time)
{
    if (_trainSoundTime != time) {
        _needsUpdate = true;
        _trainSoundTime = time;
    }
}

void DisplayController::setStationSoundTime(int time)
{
    if (_stationSoundTime != time) {
        _needsUpdate = true;
        _stationSoundTime = time;
    }
}

void DisplayController::setStatusMessage(String message, bool instant = false)
{
    _resetStatusTimer = 0;
    _statusMessage = message;
    _needsUpdate = true;

    if (instant) {
        update();
    }
}

void DisplayController::setTrainStateAndLocation(TrainState state, TrainLocation location)
{
    if (state != _trainState || location != _trainLocation) {
        _trainState = state;
        _trainLocation = location;
        _needsUpdate = true;
    }
}

// Private

void DisplayController:: drawAutoDriveTimer() {
    u8g2.drawBox(0, 11, 128, 20);
    int time;
    if (_trainState == TrainStateDriving) {
        time = 0;
    } else {
        time = (_schedulerSates[AutomaticDriveScheduler]) ? _autoDriveTime : -1;
    }
    drawTimer(F("AUTO"), F("DRIVE"), time, 28, 4, 120);
}

void DisplayController:: drawTrainSoundTimer() {
    int time = _schedulerSates[TrainSoundScheduler] ? _trainSoundTime : -1;
    drawTimer(F("TRN"), F("SND"), time, 49, 4, 56);
}
void DisplayController:: drawStationSoundTimer() {
    int time =  _schedulerSates[StationSoundScheduler] ? _stationSoundTime : -1;
    drawTimer(F("STN"), F("SND"), time, 49, 68, 56);
}

void DisplayController::drawSchedulerStates() {
    u8g2.setDrawColor(1);

    if(_schedulerSates[AutomaticDriveScheduler]) {
        u8g2.drawBox(0, 53, 42, 10);
    } else {
        u8g2.drawFrame(0, 53, 42, 10);
    }

    if(_schedulerSates[TrainSoundScheduler]) {
        u8g2.drawBox(43, 53, 42, 10);
    } else {
        u8g2.drawFrame(43, 53, 42, 10);
    }

    if(_schedulerSates[StationSoundScheduler]) {
        u8g2.drawBox(86, 53, 42, 10);
    } else {
        u8g2.drawFrame(86, 53, 42, 10);
    }

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);
    u8g2.drawStr(5,61, "A-DRIVE");
    u8g2.drawStr(46,61,"TRN-SND");
    u8g2.drawStr(90,61,"STN-SND");
}

void DisplayController::drawStatus() {
    u8g2.drawHLine(0,0,128);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2_uint_t width  = u8g2.getStrWidth(_statusMessage.c_str());
    u8g2.drawStr(64 - width / 2, 9, _statusMessage.c_str());
}

void DisplayController::drawTimer(String topLabel, String bottomLabel, int timeInSeconds, int8_t yPositionBaseLine, int8_t xPosition = 0, int8_t width = 128) {
    String time;
    if (timeInSeconds == -1) {
        time = "-";
    } else if (timeInSeconds == 0) {
        time = "";
    } else {
        char buffer[10];
        int seconds = timeInSeconds % 60;
        int minutes = timeInSeconds / 60;
        sprintf(buffer, "%d:%02d", minutes, seconds);
        time = buffer;
    }

    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);

    u8g2.drawStr(xPosition, yPositionBaseLine - 8, topLabel.c_str());
    u8g2.drawStr(xPosition, yPositionBaseLine, bottomLabel.c_str());

    u8g2.setFont(u8g2_font_crox4hb_tn);

    u8g2_uint_t textWidth  = u8g2.getStrWidth(time.c_str());
    u8g2.drawStr(xPosition + width - textWidth, yPositionBaseLine, time.c_str());
}

void DisplayController::drawDriveIndicator(int8_t x, int8_t y, int8_t w, int8_t h)
{
    u8g2.setDrawColor(0);
    u8g2.drawBox(x, y + 1, h - 2, h - 2);
    u8g2.drawBox(x + w - h + 2, y + 1, h - 2, h - 2);

    u8g2.drawBox(x, y + h/2 - 1, w / 2 - 6, 2);
    u8g2.drawBox(x + w / 2 + 6, y + h/2 - 1, w / 2 - 6, 2);

    if (_blink) {
        if (_trainLocation == TrainLocationEast) {
            u8g2.drawTriangle(x + w / 2 - 4, y-1,      x + w / 2 + 4, y + h / 2 - 1,      x + w / 2 - 4, y + h);
        } else {
            u8g2.drawTriangle(x + w / 2 + 4, y-1,      x + w / 2 - 4, y + h / 2 - 1,      x + w / 2 + 4, y + h);
        }
    }
}

int DisplayController::freeMemory() 
{
  int free_memory;

  if((int)__brkval == 0)
     free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}