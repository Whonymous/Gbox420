void calibrateLights(){
  int LastBrightness = MySettings.LightBrightness;
  bool LastLightStatus = MySettings.isLightOn;
  MaxLightReading = 0;
  MinLightReading = 1023;
  isPotGettingHigh = false;
  MySettings.isLightOn=true;
  lightCheck();
  for (int steps = 0; steps < PotStepping; steps++) 
    {stepOne();}  //Sets the digital potentiometer to low irregardless what the stored startup value is
  MySettings.LightBrightness = 0;
  isPotGettingHigh = true;  //set next step direction
  runToEnd(); //runs to highest value
  runToEnd(); //runs to lowest value (same function)
  setBrightness(LastBrightness);
  MySettings.isLightOn=LastLightStatus;
  lightCheck();  
  strncpy_P(LogMessage,(PGM_P)F("New min/max: "),LogLength);
  strcat(LogMessage,intToChar(MinLightReading));
  strcat_P(LogMessage,(PGM_P)F("/"));
  strcat(LogMessage,intToChar(MaxLightReading));
  addToLog(LogMessage);
}

void triggerCalibrateLights(){
  CalibrateLights = true;
  addToLog(F("Lights calibrating..."));  
}

void stepOne(){  //Adjust Potentiometer by one
  digitalWrite(PotUDOutPin,isPotGettingHigh);  //true=HIGH=increase brightness
  digitalWrite(PotINCOutPin,HIGH);   //signal the change to the chip
  if(MySettings.isSoundEnabled){ tone(BuzzerOutPin,MySettings.LightBrightness * 10); }
  delay(10);  //delay super oversized, https://www.intersil.com/content/dam/intersil/documents/x9c1/x9c102-103-104-503.pdf  
  digitalWrite(PotINCOutPin,LOW);
  delay(10);
  noTone(BuzzerOutPin);
  if(isPotGettingHigh && (MySettings.LightBrightness < PotStepping))  MySettings.LightBrightness++;
  else if(MySettings.LightBrightness > 0)  MySettings.LightBrightness--;
}

void setBrightness(int NewBrightness){
  strncpy_P(LogMessage,(PGM_P)F("Brightness: "),LogLength);  
  strcat(LogMessage,intToChar(NewBrightness));
  strcat_P(LogMessage,(PGM_P)F("%"));
  addToLog(LogMessage);    
  while(MySettings.LightBrightness != NewBrightness){
    if(NewBrightness < MySettings.LightBrightness)  isPotGettingHigh = false;
    else isPotGettingHigh = true;
    stepOne();
  }
}

void turnLightON(){
   MySettings.isLightOn = true;
   addToLog(F("Light ON")); 
   PlayOnSound=true;
}

void turnLightOFF(){
  MySettings.isLightOn = false;
  addToLog(F("Light OFF")); 
  PlayOffSound=true; 
}

void lightCheck(){
  if(!digitalRead(PowerButtonInPin)){ //If the power button is kept pressed down
    if(MySettings.isLightOn) turnLightOFF();
    else turnLightON();  
    }
  if(CalibrateLights){CalibrateLights=false;calibrateLights();}
  if(MySettings.isLightOn){
    digitalWrite(PowerLEDOutPin, HIGH); //Turn on Power Led on PC case if light is on
    digitalWrite(Relay8OutPin, LOW); //True turns relay ON (LOW signal activates Relay)
  }
  else {
    digitalWrite(PowerLEDOutPin, LOW); 
    digitalWrite(Relay8OutPin, HIGH); //False turns relay OFF  
  }
}

void setTimerOnOff(bool TimerState){
  MySettings.isTimerEnabled = TimerState;
  if(MySettings.isTimerEnabled){ 
    checkLightTimer();
    addToLog(F("Timer enabled"));
    PlayOnSound=true;
    }
  else {
    addToLog(F("Timer disabled"));
    PlayOffSound=true;
    }
}

void setLightsOnHour(int OnHour){
  MySettings.LightOnHour = OnHour; 
}

void setLightsOnMinute(int OnMinute){
  MySettings.LightOnMinute = OnMinute;
  checkLightTimer();
  addToLog(F("Light ON time updated")); 
}

void setLightsOffHour(int OffHour){
  MySettings.LightOffHour = OffHour;
}

void setLightsOffMinute(int OffMinute){
  MySettings.LightOffMinute = OffMinute;
  checkLightTimer();
  addToLog(F("Light OFF time updated"));
}

void checkLightTimer() {
  if(MySettings.isTimerEnabled){
    Time Now = Clock.time();  // Get the current time and date from the Real Time Clock module.
    int CombinedOnTime = MySettings.LightOnHour * 100 + MySettings.LightOnMinute;
    int CombinedOffTime = MySettings.LightOffHour * 100 + MySettings.LightOffMinute;
    int CombinedCurrentTime = Now.hr * 100 + Now.min;
    if(CombinedOnTime <= CombinedOffTime)  //no midnight turnover, Example: On 8:10, Off: 20:10
    {
      if(CombinedOnTime <= CombinedCurrentTime && CombinedCurrentTime < CombinedOffTime){
        if(MySettings.isLightOn != true) turnLightON();}
      else if(MySettings.isLightOn != false) turnLightOFF();     
    }
    else   //midnight turnover, Example: On 21:20, Off: 9:20
    {
      if(CombinedOnTime <= CombinedCurrentTime || CombinedCurrentTime < CombinedOffTime){
       if(MySettings.isLightOn != true) turnLightON();}
      else if(MySettings.isLightOn != false) turnLightOFF();    
    }
  }
}

void runToEnd(){  //Goes to Minimum or Maximum dimming, measure light intensity on the way
  int StepCounter = 0;
  while(StepCounter < PotStepping){ 
    stepOne();      //step in one direction (Initially upwards) 
    LightReading = 1023 - analogRead(LightSensorAnalogInPin);
    if(LightReading > MaxLightReading) MaxLightReading = LightReading;
    if(LightReading < MinLightReading) MinLightReading = LightReading;
    if(StepCounter % 10 == 0)  //modulo division, https://www.arduino.cc/reference/en/language/structure/arithmetic-operators/modulo/
      {  
       if(isPotGettingHigh)LogToSerials(StepCounter,false);
       else LogToSerials(PotStepping - StepCounter,false);
       LogToSerials(F("% - "),false); LogToSerials(LightReading,true);
      }  
     StepCounter++;
  }
  isPotGettingHigh= !isPotGettingHigh;  // flip the direction
}

void store(){  //store current potentiometer value in the X9C104 memory for the next power on. Write durability is only 100.000 writes, use with caution
  digitalWrite(PotINCOutPin,HIGH);
  digitalWrite(PotCSOutPin,HIGH);
  delay(50);
  digitalWrite(PotCSOutPin,LOW);
  digitalWrite(PotINCOutPin,LOW);
}

const __FlashStringHelper * lightStatusToText(){
   if(!MySettings.isLightOn) return F("ON");
   else return F("OFF");
} 

const __FlashStringHelper * isBrightToText(){
   if(isBright) return F("YES");
   else return F("NO");
}
