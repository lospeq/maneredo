#include "sleep.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include <Arduino.h>

#define SLEEP_TIMEOUT 3 * 60 * 1000  
#define WAKEUP_PIN GPIO_NUM_33

static uint32_t lastActivity = 0;

void setupSleep() {
  esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, LOW); // LOW = wakeup
  lastActivity = millis();
}

void resetSleepTimer() {
  lastActivity = millis();
}

void checkSleep() {
  if (millis() - lastActivity > SLEEP_TIMEOUT) {
    esp_deep_sleep_start();
  }
}
