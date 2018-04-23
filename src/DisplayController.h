#include <Arduino.h>
#include <U8g2lib.h>

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
        void setSoundEffectTime(int time);
        void setStatusMessage(String message);

    private:
        void drawAutoDriveTimer();
        void drawSoundTimer();
        void drawSchedulerStates();
        void drawStatus();

        boolean _needsUpdate = true;
        boolean _schedulerSates [3] = {false, false, false};
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
        int _autoDriveTime;
        int _soundEffectTime;
        String _autoDriveText;
        String _soundEffectText;
        String _statusMessage;
        

};
