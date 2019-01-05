void checkRelays(){
  if(isAeroSprayOn) digitalWrite(Relay1OutPin, LOW); else digitalWrite(Relay1OutPin, HIGH); //True turns relay ON , False turns relay OFF  (LOW signal activates Relay) 
  if(isAeroPumpOn) digitalWrite(Relay2OutPin, LOW); else digitalWrite(Relay2OutPin, HIGH);  
  if(MySettings.isPCPowerSupplyOn) digitalWrite(Relay3OutPin, LOW); else digitalWrite(Relay3OutPin, HIGH);
  if(MySettings.isInternalFanOn) digitalWrite(Relay4OutPin, LOW); else digitalWrite(Relay4OutPin, HIGH);
  if(MySettings.isInternalFanHigh) digitalWrite(Relay5OutPin, LOW); else digitalWrite(Relay5OutPin, HIGH);
  if(MySettings.isExhaustFanOn) digitalWrite(Relay6OutPin, LOW); else digitalWrite(Relay6OutPin, HIGH);
  if(MySettings.isExhaustFanHigh) digitalWrite(Relay7OutPin, LOW); else digitalWrite(Relay7OutPin, HIGH);
  if(MySettings.isLightOn) digitalWrite(Relay8OutPin, LOW); else digitalWrite(Relay8OutPin, HIGH);
}

void powerSupplyOn(){
  MySettings.isPCPowerSupplyOn = true;
  addToLog(F("Power supply ON"));
  PlayOnSound=true;
}

void powerSupplyOff(){
  MySettings.isPCPowerSupplyOn = false;
  addToLog(F("Power supply OFF"));
  PlayOffSound=true;
}

void readPowerSensor(){
  Voltage = PowerSensor.voltage(PowerSensorIP);
  Current = PowerSensor.current(PowerSensorIP);
  Power = PowerSensor.power(PowerSensorIP);
  Energy = PowerSensor.energy(PowerSensorIP) / 1000;  //total power consumption in kWh
  if(PowerOK && Voltage < 0) {
    sendEmailAlert(F("Main%20power%20loss"),F("AC%20input%20lost."));  //https://meyerweb.com/eric/tools/dencoder/  
    PowerOK = false;
    addToLog(F("AC Power lost"));
  }
  if(!PowerOK && Voltage > 0){
    sendEmailAlert(F("Main%20power%20recovered"),F("AC%20input%20recovered."));  //https://meyerweb.com/eric/tools/dencoder/  
    PowerOK = true;
    addToLog(F("AC Power revovered"));
  }
}