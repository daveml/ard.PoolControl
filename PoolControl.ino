#include "IoControl.h"
/*
 
 */


IoControl Pump;
IoControl Blower;

void setup() {
  Serial.begin(9600);  

  IoPin Pump_On  = {INPUT_PULLUP, 5, 1};
  IoPin Pump_Off = {INPUT_PULLUP, 4, 0};
  IoPin Pump_Ctrl= {OUTPUT, 3, 1};
  Pump.init(Pump_Off, Pump_On, Pump_Ctrl, 2000, 0);
  
  IoPin Blower_On  = {INPUT_PULLUP, 8, 1};
  IoPin Blower_Off = {INPUT_PULLUP, 7, 1};
  IoPin Blower_Ctrl= {OUTPUT, 6, 1};
  Blower.init(Blower_Off, Blower_On, Blower_Ctrl, 2000, 0);
}

void loop(){
  
  Pump.didChange_Handle();
  Blower.didChange_Handle();
  
}
