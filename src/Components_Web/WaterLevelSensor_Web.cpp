#include "WaterLevelSensor_Web.h"
#include "GrowBox.h"

WaterLevelSensor_Web::WaterLevelSensor_Web(const __FlashStringHelper *Name, Module *Parent, Settings::WaterLevelSensorSettings *DefaultSettings) : WaterLevelSensor(Name,Parent,DefaultSettings)
{ //constructor
  this->Parent = Parent;
  Parent->AddToWebsiteQueue_Refresh(this); //Subscribing to the Website refresh event: Calls the websiteEvent_Refresh() method
}

void WaterLevelSensor_Web::websiteEvent_Refresh(__attribute__((unused)) char *url)
{
  if (strcmp(url, "/GrowBox.html.json") == 0)
  {
    WebServer.setArgString(getWebsiteComponentName(F("Level")), getLevelGauge());
  }
}