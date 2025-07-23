#include "controls.h"
#include <Arduino.h>

const int BUTTON_PINS[NUM_BUTTONS] = {
  12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 33
};
const int JOY_X = 39;
const int JOY_Y = 36;

bool buttonStates[NUM_BUTTONS];
int joyX = 0, joyY = 0;

void setupControls() {
  for (int i = 0; i < NUM_BUTTONS; ++i) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    buttonStates[i] = false;
  }
}

int readJoystick(int pin) {
  int val = analogRead(pin);
  val += analogRead(pin);
  val += analogRead(pin);
  return round(val / 3);
}

void updateControls() {
  for (int i = 0; i < NUM_BUTTONS; ++i) {
    buttonStates[i] = (digitalRead(BUTTON_PINS[i]) == LOW);
  }
  joyX = readJoystick(JOY_X);
  joyY = readJoystick(JOY_Y);
}

bool getButtonState(int index) {
  if (index < 0 || index >= NUM_BUTTONS) return false;
  return buttonStates[index];
}

int getJoystickX() {
  return joyX;
}

int getJoystickY() {
  return joyY;
}
