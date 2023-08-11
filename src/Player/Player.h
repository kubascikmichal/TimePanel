#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Arduino.h"
#include "Tones.h"

class Player
{
private:
    int pin;
    void play(int* melody, int melody_len, int* durations, int durations_len);
public:
    Player(int pin=13);
    ~Player();
    void playCustom();
};
#endif