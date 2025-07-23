#pragma once

void setupControls();
void updateControls();

int getJoystickX();
int getJoystickY();
bool getButtonState(int index);

const int NUM_BUTTONS = 11;
