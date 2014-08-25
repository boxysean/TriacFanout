/*
  TriacFanout.cpp
  Created by Sean McIntyre, August 2014.
*/

#include "Arduino.h"
#include "TriacFanout.h"

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

    // this->zeroCross = false; //reset zero cross detection
    this->stepCounter++;
  }

  interrupts();
}         

void TriacFanout::update(int newState) {
  SPI.transfer(newState);
  // shiftOut(11, 13, MSBFIRST, this->state);
  digitalWrite(this->latchPin, HIGH);       // turn off TRIAC (and AC)
  digitalWrite(this->latchPin, LOW);       // turn off TRIAC (and AC)
} 