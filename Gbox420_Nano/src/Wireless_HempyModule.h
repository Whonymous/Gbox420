#pragma once

#include "TimeLib.h"     ///keeping track of time


///Structs for wireless communication
struct hempyCommand  ///Max 32 bytes. Template of the command sent by the Transmitter. Both Transmitter and Receiver needs to know this structure
{
   time_t Time;
   
   bool DisablePump1 = false;   
   bool TurnOnPump1 = false;   
   bool TurnOffPump1 = false;   
   int TimeOutPump1 = 0;   
   float StartWeightBucket1 = 0.0;
   float StopWeightBucket1 = 0.0;
   
   bool DisablePump2 = false;
   bool TurnOnPump2 = false;
   bool TurnOffPump2 = false;
   int TimeOutPump2 = 0;
   float StartWeightBucket2 = 0.0;
   float StopWeightBucket2 = 0.0; 
};

struct hempyResponse  ///Max 32 bytes. Template of the response sent back to the Transmitter. Both Transmitter and Receiver needs to know this structure
{
   bool OnPump1; 
   bool EnabledPump1;
   float WeightBucket1;

   bool OnPump2;
   bool EnabledPump2; 
   float WeightBucket2;
};
