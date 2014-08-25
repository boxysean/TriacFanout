#include <TimerOne.h>
#include <SPI.h>
#include "TriacFanout.h"

//#define DEBUG
#define STEPS 64

TriacFanout tf;

void setup() {
  tf.init(1, 9);
  attachInterrupt(0, zcDetect, RISING);   // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.attachInterrupt(dimCheck, tf.freqStep);
  
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

int yes = 0;

void zcDetect() { tf._zcDetect(); }
void dimCheck() { tf._dimCheck(); }

int level = 0;

void scroll() {
//  for (int i = 0; i < 1; i++) {
    tf.brightness[0] = level;
//    tf.setBrightness(i, level);
//    tf.setBrightness(i, (level + (i << 4)) & 127);
#ifdef DEBUG
    Serial.print(tf.brightness[i]); Serial.print(" ");
#endif
//  }
  
#ifdef DEBUG
  Serial.print("state "); Serial.print(tf.state);
  Serial.print(" zeroCross "); Serial.print(tf.zeroCross);
  Serial.print(" stepCounter "); Serial.print(tf.stepCounter);
  Serial.print(" yes "); Serial.print(yes);
  Serial.println();
#endif  
  
  
  level = (level+1) & 127;
  
  delay(50);
}

void fade(int sp) {
  for (int i = 0; i < STEPS*2; i++) {
    for (int j = 0; j < 8; j++) {
      if (i < STEPS) {
        tf.setBrightness(j, i);
      } else {
        tf.setBrightness(j, (STEPS*2) - i);
      }
    }
    delay(sp);
  }
}

void fancyFade(int sp) {
  for (int i = 0; i < STEPS*2; i++) {
    for (int j = 0; j < 8; j++) {
      int x = (i + j * 4) & (STEPS*2-1);
      if (x < STEPS) {
        tf.setBrightness(j, x);
      } else {
        tf.setBrightness(j, (STEPS*2) - x);
      }
    }
    
    delay(sp);
  }
}

void setLevel() {
  int x;
  
  for (int i = 0; i < 8; i++) {
    tf.setBrightness(i, 255);
  }
  
  while (1) {
    while (x = Serial.parseInt()) {
      tf.setBrightness(0, x);
      Serial.println(x);
    }
  }
}

void loop() {
//  scroll();

  for (int i = 0; i < 8; i++) {
    fancyFade(20);
  }
  
  for (int i = 0; i < 8; i++) {
    fade(20);
  }

//  setLevel();

}
