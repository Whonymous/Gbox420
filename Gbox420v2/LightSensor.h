#pragma once

#include "420Common.h"

class GrowBox;  //forward declaration

class LightSensor : public Common
{
  public:
    LightSensor(const __FlashStringHelper * Name, GrowBox * GBox, Settings::LightSensorSettings * DefaultSettings);
    void websiteEvent_Refresh(__attribute__((unused)) char * url); 
    void websiteEvent_Button(char * Button);
    void refresh_Minute();  //Called when component should refresh its state
    void report();
    bool getDark(); //Light sensor digital feedback: True(Bright) or False(Dark)
    int getReading(); //Light sensor analog feedback: 0(darkest) to 1023 (brightest)
    char * getReadingPercentage(); //Light sensor analog feedback relative to calibration values: 0 to 100%  //CALIBRATION NOT IMPLEMENTED
    char * getDarkText(bool UseWords);  
    char * getReadingText(bool IncludeMinMax); //returns the current light sensor reading. If IncludeMinMax is set to true it also includes the calibated minimum and maximum readings
    void triggerCalibration();    //Website signals to calibrate the MAX/MIN readings the next time the object gets CPU time
    
  private:
    GrowBox * GBox;
    byte * DigitalPin; //D0 - LM393 light sensor digital in
    byte * AnalogPin; //A0 - LM393 light sensor analog in
    RollingAverage * LightReading;  //keeps an average of previous readings: Smoothens sensor readings
    bool Dark;  
    bool CalibrateRequested;
    void calibrate();
    int MaxReading; // stores the highest light sensor analog reading measured during calibration
    int MinReading; //stores the lowest light sensor analog reading measured during calibration

  protected:
};
