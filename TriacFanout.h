/*
  TriacFanout.h
  Created by Sean McIntyre, August 2014.
*/

#ifndef TRIACFANOUT_H
#define TRIACFANOUT_H

#include "Arduino.h"
#include <TimerOne.h>
#include <SPI.h>

#define NUM_BULBS_PER_UNIT 8

class TriacFanout {
  public:
    TriacFanout();
    void init(int numUnits, int latchPin);
    void setBrightness(int idx, int brightness);
    void update(int newState);

    void _dimCheck();
    void _zcDetect();

    volatile int brightness[8];
    volatile int state;
    volatile bool zeroCross;  // Boolean to store a "switch" to tell us if we have crossed zero
    volatile int stepCounter;


    static int freqStep;    // This is the delay-per-brightness step in microseconds.
// It is calculated based on the frequency of your voltage supply (50Hz or 60Hz)
// and the number of brightness steps you want. 
// 
// The only tricky part is that the chopper circuit chops the AC wave twice per
// cycle, once on the positive half and once at the negative half. This meeans
// the chopping happens at 120Hz for a 60Hz supply or 100Hz for a 50Hz supply. 

// To calculate freqStep you divide the length of one full half-wave of the power
// cycle (in microseconds) by the number of brightness steps. 
//
// (1000000 uS / 120 Hz) / 128 brightness steps = 65 uS / brightness step
//
// 1000000 us / 120 Hz = 8333 uS, length of one half-wave.


  private:
    int numUnits;
    int latchPin;

    void setChannel(int idx, boolean on);
};

#endif