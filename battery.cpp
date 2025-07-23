#include "battery.h"
#include <Arduino.h>

static uint32_t lastCheck = 0;
const int checkTime = 60 * 1000;
const int transistorTime = checkTime - 500;
const int batteryPin = 34;
const int transistorPin = 32;
static int percent = 100;

const int maxReading = 2300; 
const int minReading = 1560; 
const int fullBattery = maxReading - minReading; 

void setupBatteryMonitor() {
  pinMode(batteryPin, INPUT);
  pinMode(transistorPin, OUTPUT);
  checkBattery();
}

void setTransistorState() {
  unsigned long now = millis();
  if (lastCheck == 0 || now - lastCheck > transistorTime) {
    digitalWrite(transistorPin, HIGH);
  } else {
    digitalWrite(transistorPin, LOW);
  }
}

void checkBattery() {
  setTransistorState();
  unsigned long now = millis();
  
  if (lastCheck > 0 && now - lastCheck < checkTime) return;
  lastCheck = now;

  int raw = analogRead(batteryPin);
  raw += analogRead(batteryPin);
  raw += analogRead(batteryPin);
  raw = round(raw / 3) - minReading;

  percent = round(raw * 100 / fullBattery);
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
}

int getBatteryPercent() {
  return percent;
}

bool isBatteryLow() {
  return percent < 30;
}
