/*
  TriacFanout.cpp
  Created by Sean McIntyre, August 2014.
*/

#include "Arduino.h"
#include "TriacFanout.h"

// This is the delay-per-brightness step in microseconds.
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
//
// Further modified to be 32 brightness steps (4 x 65 = 260), seems to give better results for flickering
int TriacFanout::freqStep = 260;



TriacFanout::TriacFanout() {
}

void TriacFanout::init(int numUnits = 1, int latchPin = 9) {
  if (numUnits < 1) {
    numUnits = 1;
  }

  this->numUnits = numUnits;
  this->latchPin = latchPin;

  Timer1.initialize(this->freqStep);                      // Initialize TimerOne library for the freq we need
  SPI.begin();

  pinMode(this->latchPin, OUTPUT);

  // pinMode(11, OUTPUT);
  // pinMode(13, OUTPUT);
}

void TriacFanout::setBrightness(int idx, int level) {
  brightness[idx] = level;
}

void TriacFanout::_zcDetect() {
  noInterrupts();

  this->zeroCross = true;
  this->stepCounter = 0;
  this->state = 0;

  this->update(this->state);

  interrupts();
}

// Turn on the TRIAC at the appropriate time
void TriacFanout::_dimCheck() {       
  noInterrupts();

  if (this->zeroCross) {
    bool updateNow = false;

    for (int i = 0; i < 8; i++) {
      if (this->stepCounter >= brightness[i]) {
        this->state |= (1 << i);
        updateNow = true;
      }
    }

    if (updateNow) {
      this->update(this->state);
    }

    this->stepCounter++;
  }

  interrupts();
}         

void TriacFanout::update(int newState) {
  SPI.transfer(newState);
  // shiftOut(11, 13, MSBFIRST, this->state);
  digitalWrite(this->latchPin, HIGH);
  digitalWrite(this->latchPin, LOW);
} 