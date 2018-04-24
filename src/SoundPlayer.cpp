#include "SoundPlayer.h"
#include <DFPlayer_Mini_Mp3.h>

SoundPlayer::SoundPlayer() : _mySerial(DF_PLAYER_RX, DF_PLAYER_TX)
{
}

void SoundPlayer::init()
{
    _mySerial.begin(9600);
    
    mp3_set_serial(_mySerial);
    mp3_set_device(2);  

    mp3_get_tf_sum();
    _songCount = mp3_wait_tf_sum();
}

int SoundPlayer::playRandomSound()
{
    if (_songCount <= 0) return 0;
    
    int num = getRandomSongId();
    mp3_play(num);
    return num;
}

void SoundPlayer::setVolume(int8_t volume)
{
    mp3_set_volume(volume);
}

int SoundPlayer::getSongCount()
{
    
    return _songCount;
}


int SoundPlayer::getRandomSongId()
{
    if (_songCount <= 1) return 1;
    
    int newSongId;
    do {
        newSongId = random(_songCount) + 1;
    } while(newSongId == _lastSongId);
    _lastSongId = newSongId;

    return newSongId;
}