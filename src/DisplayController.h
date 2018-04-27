#include <Arduino.h>
#include <U8g2lib.h>
#include <elapsedMillis.h>

#define STATUS_RESET_DELAY 5000

enum Scheduler {
    AutomaticDriveScheduler = 0,
    TrainSoundScheduler = 1,
    StationSoundScheduler = 2
};

class DisplayController {
    public:
        DisplayController();
        void init();
        void update();
        void setSchedulerState(Scheduler scheduler, boolean state);
        void setAutoDriveTime(int time);
        void setTrainSoundTime(int time);
        void setStationSoundTime(int time);
        void setStatusMessage(String message, bool instant = false);

        U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;

    private:
        void drawAutoDriveTimer();
        void drawTrainSoundTimer();
        void drawStationSoundTimer();
        void drawSchedulerStates();
        void drawStatus();
        void drawTimer(String topLabel, String bottomLabel, int timeInSeconds, int8_t yPositionBaseLine, int8_t xPosition = 0, int8_t width = 128);

        int freeMemory();

        boolean _needsUpdate = true;
        boolean _schedulerSates [3] = {false, false, false};
        
        int _autoDriveTime = -1;
        int _trainSoundTime = -1;
        int _stationSoundTime = -1;
        String _statusMessage;
        elapsedMillis _resetStatusTimer;
        

};
