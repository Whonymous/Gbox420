#include "Sound.h" 
#include "../../GrowBox.h"

Sound::Sound(const __FlashStringHelper * Name, GrowBox * GBox, Settings::SoundSettings * DefaultSettings):Common(Name){
  this -> GBox = GBox;
  Pin = &DefaultSettings -> Pin;
  Enabled = &DefaultSettings -> Enabled;
  pinMode(*Pin, OUTPUT);
  GBox -> AddToWebsiteQueue_Load(this);  //Subscribing to the Website load event 
  GBox -> AddToWebsiteQueue_Button(this); //Subscribing to the Website button press even 
  GBox -> AddToWebsiteQueue_Field(this); //Subscribing to the Website field submit event
  GBox -> AddToRefreshQueue_Sec(this);  //Subscribing to the Sec refresh queue: Calls the refresh_sec() method 
  logToSerials(F("Sound object created"),true,1);
}

void Sound::websiteEvent_Load(__attribute__((unused)) char * url){ //When the website is opened, load stuff once
  if (strcmp(url,"/Settings.html.json")==0){
    WebServer.setArgBoolean(getWebsiteComponentName(F("Enabled")), *Enabled);
  }
} 

void Sound::websiteEvent_Button(char * Button){ //When a button is pressed on the website
  if(!isThisMyComponent(Button)) {  //check if component name matches class. If it matches: fills ShortMessage global variable with the button function 
    return;  //If did not match:return control to caller fuction
  }
  else{ //if the component name matches with the object name   
    if (strcmp_P(ShortMessage,(PGM_P)F("Ee"))==0) { playEE(); }
  }  
}

void Sound::websiteEvent_Field(char * Field){ //When the website field is submitted
  if(!isThisMyComponent(Field)) {  //check if component name matches class. If it matches: fills ShortMessage global variable with the button function 
    return;  //If did not match:return control to caller fuction
  }
  else{ //if the component name matches with the object name   
    if(strcmp_P(ShortMessage,(PGM_P)F("Enabled"))==0) {setSoundOnOff(WebServer.getArgBoolean());}    
  }  
} 

void Sound::refresh_Sec(){
  if(GBox -> BoxSettings -> DebugEnabled) Common::refresh_Sec();
  if(*Enabled){  
    if (PlayOnSound) OnSound();
    if (PlayOffSound) OffSound();
  }
  PlayOnSound = false;
  PlayOffSound = false;
  if (PlayEE)  {PlayEE = false;EE();} 
}

void Sound::playOnSound(){
  PlayOnSound = true;
}

void Sound::playOffSound(){
  PlayOffSound = true;
} 

void Sound::OnSound(){
  if(*Enabled){ 
  tone(*Pin,500);
  delay(100);
  noTone(*Pin);
  tone(*Pin,1000);
  delay(100);
  noTone(*Pin);
  }
}

void Sound::OffSound(){
  if(*Enabled){ 
  tone(*Pin,1000);
  delay(100);
  noTone(*Pin);
  tone(*Pin,500);
  delay(100);
  noTone(*Pin);
  }
}

void Sound::setSoundOnOff(bool State){
  *Enabled = State;
  if(*Enabled){ 
    GBox -> addToLog(F("Sound enabled"));
    playOnSound();
    }
  else {
    GBox -> addToLog(F("Sound disabled"));
    playOffSound();
    }
}

//EE Section, can delete everything below if you need to save space

void Sound::playEE(){
  PlayEE = true;
}

const PROGMEM int Sound::melody[] = {   //https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
  2637, 2637, 0, 2637,
  0, 2093, 2637, 0,
  3136, 0, 0,  0,
  1568, 0, 0, 0,

  2093, 0, 0, 1568,
  0, 0, 1319, 0,
  0, 1760, 0, 1976,
  0, 1865, 1760, 0,

  1568, 2637, 3136,
  3520, 0, 2794, 3136,
  0, 2637, 0, 2093,
  2349, 1976, 0, 0,

  2093, 0, 0, 1568,
  0, 0, 1319, 0,
  0, 1760, 0, 1976,
  0, 1865, 1760, 0,

  1568, 2637, 3136,
  3520, 0, 2794, 3136,
  0, 2637, 0, 2093,
  2349, 1976, 0, 0,
  
  0, 0, 0,
  
  262, 523, 220, 440,
  233, 466, 0,
  0,
  262, 523, 220, 440,
  233, 466, 0,
  0,
  175, 349, 147, 294,
  156, 311, 0,
  0,
  175, 349, 147, 294,
  156, 311, 0,
  0, 311, 277, 294,
  277, 311,
  311, 208,
  196, 277,
  262, 370, 349, 165, 466, 440,
  415, 311, 247,
  233, 220, 208
};

const PROGMEM byte Sound::tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
    
  30, 30, 30,
  
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10  
};

void Sound::EE() { 
  GBox -> addToLog(F("♬Easter egg♬"));
  //int size = sizeof(melody) / sizeof(int);  //this is equal 134: No sense calculating it every time, if you change the melody could come handy.
  for (int thisNote = 0; thisNote < 134; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / (byte)pgm_read_word(&tempo[thisNote]);  //tempo is stored in PROGMEM (Flash), cannot read from it as RAM array (temp[thisNote] would not work) //https://forum.arduino.cc/index.php?topic=106603.0
    buzz((int)pgm_read_word(&melody[thisNote]), noteDuration);
    // to distinguish the notes, set a minimum time between them.
    delay(noteDuration);      
    // stop the tone playing:
    buzz(0, noteDuration); 
    wdt_reset(); //Reset Watchdog timeout to avoid Arduino reseting while playing the song   
  }
}

void Sound::buzz( uint32_t frequency, uint32_t length) {
  digitalWrite(13, HIGH);
  uint32_t delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  uint32_t numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (uint32_t i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(*Pin, HIGH); // write the Sound pin high to push out the diaphragm
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(*Pin, LOW); // write the Sound pin low to pull back the diaphragm
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }  
  digitalWrite(13, LOW);
}