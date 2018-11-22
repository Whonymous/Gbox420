void readAeroPressure(){
  float  Reading=0;
  for(byte i=0;i<50;i++) { 
   Reading+=analogRead(PressureSensorInPin);
   delay(20);
  }
  Reading = Reading /50;
  float Voltage = ((float)Reading) * 5 / 1024 ;
  AeroPressure = (2.7*(Voltage-MySettings.AeroOffset)); // unit: bar / 100kPa
  AeroPressurePSI = AeroPressure * 14.5038; 
}

void calibratePressureSensor(){  //Should only be called when there is 0 pressure
  float sum = 0;
  for(byte i = 0; i<50;i++){
  sum += analogRead(PressureSensorInPin) ; 
  delay(10);
  }  
  MySettings.AeroOffset = (sum/50)*5/1024; //Reads voltage at 0 pressure
  strncpy(LogMessage,"Pressure sensor offset: ",LogLength);
  strcat(LogMessage,floatToChar(MySettings.AeroOffset));
  addToLog(LogMessage);
}

void setAeroDuration(int duration){  
  MySettings.AeroDuration = WebServer.getArgInt() * 1000;
  AeroSprayTimer = millis();  
}

void setAeroInterval(int interval){  
  MySettings.AeroInterval = interval * 60000; 
  AeroSprayTimer = millis(); 
}

void aeroSprayNow(){   
  if(MySettings.isAeroSprayEnabled){
    AeroSprayTimer = millis();
    isAeroSprayOn = true;
    PlayOnSound = true;
    relayCheck();
    addToLog("Aeroponics Spraying");
    }
}

void aeroSprayOff(){   
    isAeroSprayOn = false;    
    relayCheck();
    addToLog("Aeroponics Spray OFF");
}

void checkAeroStatus(){
 checkAeroSprayTimer();
 checkAeroPump();
}

void checkAeroSprayTimer(){
 if(isAeroSprayOn)    { //if spray is on
    if(millis() - AeroSprayTimer >= MySettings.AeroDuration){  //if time to stop spraying
      isAeroSprayOn = false;
      Serial.println("Stopping spray");
      PlayOffSound = true;
      AeroSprayTimer = millis();
    }
  }
  else{ //if spray is off
    if(millis() - AeroSprayTimer >= MySettings.AeroInterval){ //if time to start spraying
      if(MySettings.isAeroSprayEnabled){
      isAeroSprayOn = true;
      Serial.println("Starting spray");
      PlayOnSound = true;
      AeroSprayTimer = millis();
      }
    }
  }
}

void checkAeroPump(){
  if(isAeroPumpOn){
    readAeroPressure();   
    if(AeroPressure >= MySettings.AeroPressureHigh){
      aeroPumpOff();
    }
    else if (millis() - AeroPumpTimer >= AeroPumpTimeout){
      isAeroPumpOn = false;      
      isAeroPumpDisabled = true;
      addToLog("Pump failed, alerting");
      PlayOffSound = true;
      SendEmailAlert(PumpAlertDeviceID);
    }
  }
  else{
    if(AeroPressure <= MySettings.AeroPressureLow && !isAeroPumpDisabled){
      addToLog("Turning on pump");
      aeroPumpOn();
    }
  }
}

void aeroPumpRefill(){  
  addToLog("Refilling pressure tank");
  aeroPumpOn();
}

void aeroPumpOn(){
  isAeroPumpDisabled = false;
  isAeroPumpOn = true;
  AeroPumpTimer = millis();      
  PlayOnSound = true;
}

void aeroPumpOff(){
  isAeroPumpOn = false;
  addToLog("Turning off pump");
  PlayOffSound = true;
}

void aeroPumpReset(){
  isAeroPumpDisabled = false;
  isAeroPumpOn = false;
  addToLog("Pump reset");
}

