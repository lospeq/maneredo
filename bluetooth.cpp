#include "bluetooth.h"
#include "controls.h"
#include "sleep.h"
#include "battery.h"
#include <BleGamepad.h>
#include <Arduino.h>

static bool lastStates[NUM_BUTTONS] = {false};
static int lastX = 0, lastY = 0;
const int DEADZONE = 100;

BleGamepad bleGamepad;

void setupBluetooth() {
  bleGamepad = BleGamepad("maneredo", "jitaku", getBatteryPercent());
  bleGamepad.begin();
}

void updateBleGamepad() {
  if (!bleGamepad.isConnected()) return;
  bool changed = false;

  for (int i = 0; i < NUM_BUTTONS; ++i) {
    bool pressed = getButtonState(i);
    if (pressed != lastStates[i]) {
      changed = true;
      break;
    }
  }

  int x = getJoystickX();
  int y = getJoystickY();
  if (abs(x - lastX) > DEADZONE || abs(y - lastY) > DEADZONE) {
    changed = true;
  }

  if (!changed) return;
  resetSleepTimer(); 
  bleGamepad.setBatteryLevel(getBatteryPercent());

  for (int i = 0; i < NUM_BUTTONS; ++i) {
    bool pressed = getButtonState(i);
    if (pressed != lastStates[i]) {
      if (pressed) {
        bleGamepad.press(i + 1);
      } else {
        bleGamepad.release(i + 1);
      }
      lastStates[i] = pressed;
    }
  }

  if (abs(x - lastX) > DEADZONE || abs(y - lastY) > DEADZONE) {
    bleGamepad.setLeftThumb(map(x, 0, 4095, 0, 32767),  map(y, 0, 4095, 0, 32767));
    lastX = x;
    lastY = y;
  }
}

bool isConnected() {
  return bleGamepad.isConnected();
}
