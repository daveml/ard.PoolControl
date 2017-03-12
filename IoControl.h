#ifndef IOCONTROL_H
#define IOCONTROL_H

#include "Arduino.h"
/*
 */

struct IoPin
{
   int Mode; // Pin Mode
   byte Idx; // Pin Index
   boolean IsActiveLow;
};

// I/O Control class
// Provides discrete binary output control based on 2 discrete input controls
class IoControl
{
public:
  void init(
    IoPin PinIn_Off, 
    IoPin PinIn_On,
    IoPin PinOut_Ctrl,
    int DebounceDelay_On,
    int DebounceDelay_Off
    )
  {
    // Store configuration

    pinOn = PinIn_On;
    pinOff = PinIn_Off;
    pinCtrl = PinOut_Ctrl;
    debounceDelay_On = DebounceDelay_On;
    debounceDelay_Off = DebounceDelay_Off;
    
    // Initialization
    pinMode(pinOn.Idx, pinOn.Mode);
    pinMode(pinOff.Idx, pinOff.Mode);
    pinMode(pinCtrl.Idx, pinCtrl.Mode);
    
    debounceTime = millis();
    setCtrlOutput(LOW);
  }
  
  // Control output state is logically handle as active high.
  // We use this function to set the actual state as is appropriate
  boolean didSet_CtrlOutput(boolean NewState)
  {
    // If changing to active state we apply the debounce logic
    if(NewState == HIGH)
    {
      if((millis() - debounceTime) < debounceDelay_On)
      {
        return false;
      }
    } 
    else {
      if((millis() - debounceTime) < debounceDelay_Off)
      {
        return false;
      }
    }
    
    setCtrlOutput(NewState);

    debounceTime = millis();
    return true;
  }

  void setCtrlOutput(boolean NewState)
  {
    // Set the output based on active high or low
    if(pinCtrl.IsActiveLow)
    {
      digitalWrite(pinCtrl.Idx, !NewState);
    } else
    {
      digitalWrite(pinCtrl.Idx, NewState);
    }
  }

  boolean didChange_Handle()
  {
    boolean LastState = isActive_Pin(pinCtrl);
    
    if(isActive_Pin(pinOff))
    {
      didSet_CtrlOutput(LOW);   
    }
    else
    if(isActive_Pin(pinOn) && !isActive_Pin(pinOff))
    {
      didSet_CtrlOutput(HIGH);
    }
    
    return (LastState != isActive_Pin(pinCtrl));
  }

  boolean isActive_Pin(IoPin Pin)
  {
    boolean CurrentState = ((!Pin.IsActiveLow) == digitalRead(Pin.Idx));
    return CurrentState;
  }

  IoPin pinOn;
  IoPin pinOff;
  IoPin pinCtrl;
  int debounceDelay_On;
  int debounceDelay_Off;
  unsigned long debounceTime;
};

#endif
