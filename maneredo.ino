
#include "bluetooth.h"
#include "controls.h"
#include "led.h"
#include "battery.h"
#include "sleep.h"

void setup() {
  setupControls();
  setupBatteryMonitor();
  setupBluetooth();
  setupLed();
  setupSleep();
}

void loop() {
  updateControls();    
  checkBattery();  
  updateBleGamepad();    
  updateLedState();      
  checkSleep();         
}
