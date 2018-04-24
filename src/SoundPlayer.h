#include <Arduino.h>
#include <SoftwareSerial.h>

#define DF_PLAYER_RX 10
#define DF_PLAYER_TX 11
class SoundPlayer {
    public:
        SoundPlayer();
        void init();
        int playRandomSound();
        void setVolume(int8_t volume);
        int getSongCount();

    private:
        int8_t _songCount;
        int _lastSongId;
        SoftwareSerial _mySerial;
        int getRandomSongId();
};
