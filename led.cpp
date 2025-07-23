#include <Arduino.h>
#include "led.h"
#include "bluetooth.h"
#include "battery.h"

const int ledPinG = 26;
const int ledPinB = 27;

const int freq = 5000;
const int resolution = 12;  
const int resValue = (1 << resolution) - 1;  

const int ledBright = (int)resValue * 0.95;
const int ledOff = resValue;

const int pulseSteps = 100;   
const int pulseTime = 2500;
const int pulsePause = 3000;   
const int pulseStepTime = pulseTime / pulseSteps;              
const int pulseStepDelta = round((resValue - ledBright) / pulseSteps);               
static int pulseValue = ledBright;
static bool pulseAscend = true;
static bool isPulsePause = false;

static uint32_t stamp = 0;
static bool blinkOn = false;

const int greenChannel = 0;
const int blueChannel = 1;

LedMode currentMode;

void writePWM(int value) {
  ledcWrite(ledPinG, value); 
  ledcWrite(ledPinB, value); 
}

void setupLed() {
  ledcAttach(ledPinG, freq, resolution);
  ledcAttach(ledPinB, freq, resolution);
}

void pulse() {
  uint32_t now = millis();

  if (isPulsePause) {
    if (now - stamp < pulsePause) {
      writePWM(ledOff);
      return;
    } else {
      isPulsePause = false;
      pulseAscend = true;
      pulseValue = ledOff;
      stamp = now;
    }
  }

  if (now - stamp >= pulseStepTime) {
    writePWM(pulseValue);

    if (pulseAscend) {
      pulseValue -= pulseStepDelta;
      if (pulseValue <= ledBright) {
        pulseValue = ledBright;
        pulseAscend = false;
      }
    } else {
      pulseValue += pulseStepDelta;
      if (pulseValue >= ledOff) {
        pulseValue = ledOff;
        isPulsePause = true;
      }
    }

    stamp = now;
  }
}

void blink() {
  uint32_t now = millis();
  if (now - stamp > 500) {  
    blinkOn = !blinkOn;
    stamp = now;
    writePWM(blinkOn ? ledOff : ledBright); 
  }
}


void updateLedState() {
  if (isBatteryLow()) {
    if (currentMode != LedMode::Blink) {
      currentMode = LedMode::Blink;
      stamp = millis();       
      blinkOn = false;        
    }
    blink();
    return;
  }

  if (isConnected()) {
    if (currentMode != LedMode::Pulse) {
      currentMode = LedMode::Pulse;
      isPulsePause = false;
      pulseAscend = true;
      pulseValue = ledBright;
      stamp = millis();
    }
    pulse();
  } else if (currentMode != LedMode::Static) {
    currentMode = LedMode::Static;
    writePWM(ledBright);
  }
}
