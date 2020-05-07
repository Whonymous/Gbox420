#include "WaterTempSensor_Web.h"

WaterTempSensor_Web::WaterTempSensor_Web(const __FlashStringHelper *Name, Module_Web *Parent, Settings::WaterTempSensorSettings *DefaultSettings) : Common(Name), WaterTempSensor(Name,Parent,DefaultSettings), Common_Web(Name)
{ ///constructor
  this->Parent = Parent;
  this->Name = Name;
  Parent->addToReportQueue(this);          
  Parent->addToRefreshQueue_Minute(this); 
  Parent->addToWebsiteQueue_Refresh(this); 
}

void WaterTempSensor_Web::websiteEvent_Refresh(__attribute__((unused)) char *url)
{
  if (strncmp(url, "/G",2) == 0)
  {
    WebServer.setArgString(getComponentName(F("T")), getTempText(true));
  }
}