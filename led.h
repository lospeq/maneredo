#pragma once

void setupLed();
void pulse();
void blink();
void updateLedState();

enum class LedMode { Pulse, Blink, Static };