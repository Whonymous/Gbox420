#pragma once

#include "420Common.h"
#include "420Module.h"

///Fan controller (2 speed)

class Fan : virtual public Common
{
public:
  Fan(const __FlashStringHelper *Name, Module *Parent, Settings::FanSettings *DefaultSettings);
  void refresh_Minute();
  void report();
  char *fanSpeedToText();
  char *fanSpeedToNumber();

private:  

protected:
  Module *Parent;
  bool *State;
  bool *HighSpeed;
  uint8_t *OnOffPin;
  uint8_t *SpeedPin;
  void checkFanStatus();
  void TurnOff();
  void SetLowSpeed();
  void SetHighSpeed();
};