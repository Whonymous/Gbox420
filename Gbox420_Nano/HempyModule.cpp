#include "HempyModule.h"
#include "src/Components/DHTSensor.h"
#include "src/Components/Sound.h"
#include "src/Components/WeightSensor.h"
#include "src/Components/PHSensor.h"
#include "src/Components/WaterTempSensor.h"
#include "src/Components/WaterLevelSensor.h"


HempyModule::HempyModule(const __FlashStringHelper *Name, Settings::HempyModuleSettings *DefaultSettings) : Common(Name), Module()
{ //Constructor
  this->Name = Name;
  Sound1 = new Sound(F("Sound1"), this, &ModuleSettings->Sound1); //Passing ModuleSettings members as references: Changes get written back to ModuleSettings and saved to EEPROM. (byte *)(((byte *)&ModuleSettings) + offsetof(Settings, VARIABLENAME))
  DHT1 = new DHTSensor(F("DHT1"), this, &ModuleSettings->DHT1);
  PHSensor1 = new PHSensor(F("PHSensor1"), this, &ModuleSettings->PHSensor1);
  WaterTemp1 = new WaterTempSensor(F("WaterTemp1"), this, &ModuleSettings->WaterTemp1);
  WaterLevel1 = new WaterLevelSensor(F("WaterLevel1"), this, &ModuleSettings->WaterLevel1);
  Weight1 = new WeightSensor(F("Weight1"), this, &ModuleSettings->Weight1);
  Weight2 = new WeightSensor(F("Weight2"), this, &ModuleSettings->Weight2);
  
  addToRefreshQueue_FiveSec(this);     //Subscribing to the 5 sec refresh queue: Calls the refresh_FiveSec() method
  addToRefreshQueue_Minute(this);      //Subscribing to the 1 minute refresh queue: Calls the refresh_Minute() method
  addToRefreshQueue_QuarterHour(this); //Subscribing to the 30 minutes refresh queue: Calls the refresh_QuarterHour() method
  logToSerials(F("HempyModule object created, refreshing..."), true, 0);
  runAll();
  addToLog(F("HempyModule initialized"), 0);
}

void HempyModule::refresh_FiveSec()
{
  if (*Debug)
    Common::refresh_FiveSec();
  if (RefreshAllRequested)
  {
    RefreshAllRequested = false;
    runAll();
  }  
  runReport();
}

void HempyModule::refresh_Minute()
{
  if (*Debug)
    Common::refresh_Minute();  
}

void HempyModule::refresh_QuarterHour()
{
  if (*Debug)
    Common::refresh_QuarterHour(); 
  Sound1 -> playEE();
}

//////////////////////////////////////////////////////////////////
//Settings
void HempyModule::setDebugOnOff(bool State)
{
  *Debug = State;
  if (*Debug)
  {
    addToLog(F("Debug enabled"));
    Sound1->playOnSound();
  }
  else
  {
    addToLog(F("Debug disabled"));
    Sound1->playOffSound();
  }
}

void HempyModule::setMetric(bool MetricEnabled)
{
  if (MetricEnabled != *Metric)
  { //if there was a change
    *Metric = MetricEnabled;
    //ModuleSettings -> IFanSwitchTemp = convertBetweenTempUnits(ModuleSettings -> IFanSwitchTemp);
    //Pres1->Pressure->resetAverage();
    DHT1->Temp->resetAverage();
   // WaterTemp1->Temp->resetAverage();
    RefreshAllRequested = true;
  }
  if (*Metric)
    addToLog(F("Metric units"));
  else
    addToLog(F("Imperial units"));
}