#pragma once

/*! 
 *  \brief     Use this to change the Default Settings for each component. Loaded when the Arduino starts, updated by user interaction on the website.
 *  \details   The Settings object is stored in EEPROM and kept between reboots. 
 *  \author    GrowBoxGuy
 *  \version   4.20
 *  \warning   Only use these in the setup() function, or when a user initiated change is stored. EEPROM has a write limit of 100.000 cycles
 *  \attention Never use the funtions defined here in loop() or a repeating cycle! EEPROM would wear out very fast
 *  \attention Update the Version number when you make change to the structure in the SAVED TO EEPROM secton. This will overwrite the EEPROM settings with the sketch defaults.
 */

static const uint8_t Version = 7; ///< Increment this when you make a change in the SAVED TO EEPROM secton

///State machine - Defining possible states
  enum PumpStates {DISABLED, IDLE, PRIMING, RUNNING, BLOWOFF, MIXING};
  //enum HempyState { DRY, WATERING};
  //enum AeroState { SPRAYING };

///THIS SECTION DOES NOT GET STORED IN EEPROM:

  ///Global constants
  static const uint8_t MaxTextLength = 32;      ///Default char * buffer for storing a word + null terminator. Memory intense!
  static const uint8_t MaxShotTextLength = 64; ///Default char * buffer for storing mutiple words. Memory intense!
  static const int MaxLongTextLength = 256; ///Default char * buffer for storing a long text. Memory intense!
  static const uint8_t QueueDepth = 8;               ///Limits the maximum number of active modules. Memory intense!
  static const uint8_t RollingAverageDepth = 10;     ///Limits the maximum number of active modules. Memory intense!

  ///Global variables
  extern char LongMessage[MaxLongTextLength];  ///temp storage for assembling long messages (REST API, MQTT API)
  extern char ShortMessage[MaxShotTextLength]; ///temp storage for assembling short messages (Log entries, Error messages)
  extern char CurrentTime[MaxTextLength];      ///buffer for storing current time in text

  ///nRF24L01+ wireless receiver
  static const uint8_t WirelessCSNPin = 9;   //< Pre-connected on RF-Nano
  static const uint8_t WirelessCEPin = 10;   //< Pre-connected on RF-Nano
  static const uint8_t WirelessChannel[6] = {"Hemp1"};    ///This needs to be unique and match with the Name of the HempyModule_Web object in the Main module
  static const uint8_t WirelessPayloadSize = 32; //Size of the wireless packages exchanged with the Main module. Max 32 bytes are supported on nRF24L01+
  static const uint16_t WirelessMessageTimeout = 500; //Default 0.5sec -  One package should be exchanged within this timeout (Including retries and delays)

///SAVED TO EEPROM - Before uploading the schetch increase the Version variable to override whatever is stored in the Arduino's EEPROM 
  typedef struct
  {
    bool Debug = true;          ///Logs debug messages to serial and web outputs
    bool Metric = true;   ///Switch between Imperial/Metric units. If changed update the default temp and pressure values too.
    
    struct HempyBucketSettings
    {
      HempyBucketSettings( bool WeightBasedWatering = false, float StartWeight = 0.0, float StopWeight = 0.0,  bool TimerBasedWatering = false, uint16_t WateringInterval = 0, uint16_t WateringDuration = 0) : WeightBasedWatering(WeightBasedWatering), StartWeight(StartWeight), StopWeight(StopWeight), TimerBasedWatering(TimerBasedWatering), WateringInterval(WateringInterval), WateringDuration(WateringDuration)   {}
      bool WeightBasedWatering;  //Enable/Disable weight based watering
      float StartWeight; ///Start watering below this weight
      float StopWeight;  ///Stop watering above this weight
      bool TimerBasedWatering; //Enable/Disable timer based watering
      uint16_t WateringInterval;   //Water every X minutes
      uint16_t WateringDuration;  //Water for X seconds
    };
    struct HempyBucketSettings Bucket1 = { .WeightBasedWatering = true, .StartWeight = 4.2, .StopWeight = 6.9, .TimerBasedWatering = false, .WateringInterval = 1440, .WateringDuration = 30};
    struct HempyBucketSettings Bucket2 = { .WeightBasedWatering = true, .StartWeight = 4.2, .StopWeight = 6.9, .TimerBasedWatering = false, .WateringInterval = 1440, .WateringDuration = 30};

    struct HempyModuleSettings{
      //HempyModuleSettings() :  {}     
    };  
    struct HempyModuleSettings HempyMod1 = {};  ///Default settings for the Hempy Module

    struct SoundSettings
    {
      SoundSettings(uint8_t Pin = 0) : Pin(Pin) {}
      uint8_t Pin;            ///PC buzzer+ (red)
      bool Enabled = true; ///Enable PC speaker / Piezo buzzer
    };
    struct SoundSettings Sound1 = {.Pin = 2};  ///Default settings for the  Sound output

   struct WaterPumpSettings
    {
      WaterPumpSettings(uint8_t PumpPin = 0, bool PumpPinNegativeLogic = false, uint8_t BypassSolenoidPin = 255, bool BypassSolenoidNegativeLogic = false, bool PumpEnabled = false, int PumpTimeOut = 0, int PrimingTime = 0, int BlowOffTime = 0) : PumpPin(PumpPin), PumpPinNegativeLogic(PumpPinNegativeLogic), BypassSolenoidPin(BypassSolenoidPin), PumpEnabled(PumpEnabled), PumpTimeOut(PumpTimeOut), PrimingTime(PrimingTime), BlowOffTime(BlowOffTime)  {}
      uint8_t PumpPin;         ///< Pump relay pin
      bool PumpPinNegativeLogic;  ///Set to true if Relay/MOSFET controlling the power to the pump requires LOW signal to Turn ON
      uint8_t BypassSolenoidPin;        ///< Bypass solenoid relay pin [optional]
      bool BypassSolenoidNegativeLogic;  ///Set to true if Relay/MOSFET controlling the power to the solenoid requires LOW signal to Turn ON [optional]
      bool PumpEnabled; ///< Enable/disable pump. false= Block running the pump
      int PumpTimeOut;   ///< (Sec) Max pump run time        
      int PrimingTime;    ///< (Sec) Only if BypassSolenoid is present. For how long to keep the bypass solenoid on when starting the pump - Remove air bubbles from pump intake side
      int BlowOffTime;     ///< (Sec) Only if BypassSolenoid is present. For how long to open the bypass solenoid on after turning the pump off - Release pressure from pump discharge side
    };
    struct WaterPumpSettings HempyPump1 = {.PumpPin = 3, .PumpPinNegativeLogic = true, .PumpEnabled = true, .PumpTimeOut = 420}; ///< Pumps do not need a bypass solenoid
    struct WaterPumpSettings HempyPump2 = {.PumpPin = 4, .PumpPinNegativeLogic = true, .PumpEnabled = true, .PumpTimeOut = 420}; ///< Pumps do not need a bypass solenoid
   
    struct WeightSensorSettings
    {
      WeightSensorSettings(uint8_t DTPin = 0, uint8_t SCKPin = 0, long Offset = 0, float Scale = 0.0) : DTPin(DTPin), SCKPin(SCKPin), Offset(Offset), Scale(Scale) {}
      uint8_t DTPin;     ///Weight sensor DT pin
      uint8_t SCKPin; ///Weight sensor SCK pin
      long Offset; ///Reading at 0 weight on the scale
      float Scale;  ///Scale factor      
    };
    struct WeightSensorSettings Weight1 = {.DTPin = 5, .SCKPin = 6, .Offset=47748, .Scale = -125707.00}; ///Update the calibration values here for Weight Sensor 1
    struct WeightSensorSettings Weight2 = {.DTPin = 7, .SCKPin = 8, .Offset=-116697, .Scale = -124397.67}; ///Update the calibration values here for Weight Sensor 2

    uint8_t CompatibilityVersion = Version; ///Should always be the last value stored.
  } Settings;

///////////////////////////////////////////////////////////////
///EEPROM related functions - Persistent storage between reboots
///Use cautiously, EEPROM has a write limit of 100.000 cycles - Only use these in the setup() function, or when a user initiated change is stored 

void saveSettings(Settings *ToSave);
Settings *loadSettings(bool ResetEEPROM = false );
void restoreDefaults(Settings *ToOverwrite);