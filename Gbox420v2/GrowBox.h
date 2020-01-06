#pragma once
#include "420Common.h"
#include "ELClientRest.h" //ESP-link - REST API

//forward declaration of classes
class DHTSensor;  
class LightSensor;
class Lights;
class Sound;
class Fan;
class PowerSensor;
//class PowerSensorV3;  //Only for PZEM004T V3.0
class PHSensor;
class LightSensor;
class PressureSensor;
class Aeroponics_NoTank;
class Aeroponics_Tank;
class WaterTempSensor;
class WaterLevelSensor;

extern ELClientRest PushingBoxRestAPI;

class GrowBox : public Common
{ 
  public:
    GrowBox(const __FlashStringHelper * Name, Settings *BoxSettings); //constructor
    Settings * BoxSettings;
    Sound * Sound1; //Pointer to a Piezo speaker - sound feedback
    Fan * InFan;  //Internal fan
    Fan * ExFan; //Exhaust fan
    DHTSensor * InDHT;  //Pointer to a Digital Humidity Sensor object measuring the internal temperature of the grow box
    DHTSensor * ExDHT; //Pointer to a Digital Humidity Sensor object measuring the external temperature of the grow box
    LightSensor * LightSensor1; //Pointer to a Light Sensor object measuring light intensity in the grow box
    Lights * Light1;  //Pointer to a Light assembly 
    PowerSensor * Power1;
    //PowerSensorV3 * Power1;  //Only for PZEM004T V3.0
    PressureSensor * Pressure1;
    PHSensor * PHSensor1;
    Aeroponics_Tank * Aero_T1;
    Aeroponics_NoTank * Aero_NT1;
    WaterTempSensor * WaterTemp1;
    WaterLevelSensor * WaterLevel1;
    void websiteEvent_Load(__attribute__((unused)) char * url);
    void websiteEvent_Refresh(__attribute__((unused)) char * url);
    void websiteEvent_Button(char * Button);
    void websiteEvent_Field(char * Field);
    void refresh_FiveSec();
    void refresh_HalfHour();
    void refreshAll(bool AddToLog);
    bool refreshRequest = true; 
 
    void runReport(); 
    void runSec();
    void runFiveSec();
    void runMinute();
    void runHalfHour(); 

    char * eventLogToJSON(bool Append); //Creates a JSON array: ["Log1","Log2","Log3",...,"LogN"]  
    void addToLog(const __FlashStringHelper* Text,byte indent=3);
    void addToLog(const char * Text,byte indent=3);
    void setDebugOnOff(bool State);
    void setMetricSystemEnabled(bool MetricEnabled); 
    void setPushingBoxLogRelayID(char * ID);
    void ReportToGoogleSheets(bool CalledFromWebsite);
    void setReportToGoogleSheetsOnOff(bool State);
  
    void AddToReportQueue(Common* Component);
    void AddToRefreshQueue_Sec(Common* Component);
    void AddToRefreshQueue_FiveSec(Common* Component);
    void AddToRefreshQueue_Minute(Common* Component);
    void AddToRefreshQueue_HalfHour(Common* Component);
    void AddToWebsiteQueue_Load(Common* Component);
    void AddToWebsiteQueue_Refresh(Common* Component);
    void AddToWebsiteQueue_Button(Common* Component);
    void AddToWebsiteQueue_Field(Common* Component);
    Common* ReportQueue[QueueDepth];
    Common* RefreshQueue_Sec[QueueDepth];
    Common* RefreshQueue_FiveSec[QueueDepth]; 
    Common* RefreshQueue_Minute[QueueDepth];
    Common* RefreshQueue_HalfHour[QueueDepth];
    Common* WebsiteQueue_Load[QueueDepth];
    Common* WebsiteQueue_Refresh[QueueDepth]; 
    Common* WebsiteQueue_Button[QueueDepth];
    Common* WebsiteQueue_Field[QueueDepth];
    byte reportQueueItemCount = 0;    //Tracking queue item count
    byte refreshQueueItemCount_Sec = 0;
    byte refreshQueueItemCount_FiveSec = 0;
    byte refreshQueueItemCount_Minute = 0;
    byte refreshQueueItemCount_HalfHour = 0;
    byte WebsiteQueueItemCount_Load = 0;
    byte WebsiteQueueItemCount_Refresh = 0;
    byte WebsiteQueueItemCount_Button = 0;
    byte WebsiteQueueItemCount_Field = 0;
      
  private: 

  protected:
};

//WEBSITE COMPONENT
/*
<div class="card" style="width:90%">
  <h1>Settings</h1>
  <button id="GBox1_RestoreDefaults" type="button" style="margin-top:0px" title="Restore settings to sketch defaults">Restore defaults</button>
  <form><label><input type="checkbox" name="GBox1_DebugEnabled" title="Debug entres on Serial log">Debug enabled </label><input type="submit" value="Set"></form>
  <form><label><input type="checkbox" name="GBox1_MetricSystemEnabled" title="Use imperial units(F,PSI) or metric (C,bar)">Metric units </label><input type="submit" value="Set"></form>				
</div>

<div class="card" style="width:90%">
  <h1>Reporting</h1>
  <button id="GBox1_ReportTrigger" type="button" style="margin-top:0px">Report to console</button>			
  <form><label><input type="checkbox" name="GBox1_GoogleSheetsEnabled">Google Sheets </label><input type="submit" value="Set"> <button id="GBox1_GoogleSheetsTrigger" type="button" style="margin-top:0px">Trigger</button></form>
  <form>RelayID:<input type="text" name="GBox1_PushingBoxLogRelayID"  style="width: 150px;" title="DeviceID of the PushingBox logging scenario"/><input type="submit" value="Set"></form>
</div>

<div class="pure-u-1 pure-u-md-1-2" style="max-width:370px">
  <div class="card" style="width:90%">	
    <h1><span id="Time"></span></h1>				
    <ul id="list_SerialLog"></ul>
    <button id="GBox1_Refresh" type="button" style="margin-top:0px" title="Updates the status of every module">Refresh</button>	
  </div>					
</div>
*/