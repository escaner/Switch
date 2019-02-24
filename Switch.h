/*
 *   Switch.h - Library to handle switches and buttons.
 *   Created by Escaner, 2019.
 */

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include <Arduino.h>


/*
 *   This library performs debounce for switches and can provide either
 *  the switch value or a flank.
 */

class Switch
{
public:
  // Defatult value for the bouncing stabilization time in microseconds
  static const unsigned long DEFAULT_DEBOUNCE_TIME = 250UL;

  // HIGH & LOW macros defined in Arduino.h
  enum Flank: int8_t
  {
    FLANK_FALLING = -HIGH,
    FLANK_NONE = LOW,
    FLANK_RISING = HIGH
  };

  // Public methods
  Switch(uint8_t State, unsigned long DebounceTime = DEFAULT_DEBOUNCE_TIME);
  uint8_t updateState(uint8_t ReadValue);
  inline Flank updateFlank(uint8_t ReadValue);

protected:
  const unsigned long _DebounceTime;  // Time of stable signal to skip bounces
  unsigned long _LastBounceTime;      // Time of last update to _BouncingAway
  bool _BouncingAway;                 // Last read of switch including bounces
  uint8_t _State;                     // Switch state discarding bounces
};


/*
 *   Updates the switch internal status with the latest read value, keeping
 *  track of bounces by calling updateState() and returns the resulting
 *  generated flank.
 *   Parameters:
 *   * ReadValue (LOW, HIGH): value read in the corresponding pin
 *   Returns: flank produced by this update
 *   * FLANK_FALLING: switch state went from HIGH to LOW
 *   * FLANK_NONE: switch state did not change
 *   * FLANK_RISING: switch state went from LOW to HIGH
 */
inline Switch::Flank Switch::updateFlank(uint8_t ReadValue)
{
  uint8_t PrevState, NewState;

  // Save current state
  PrevState = _State;

  // Update state
  NewState = updateState(ReadValue);

  // Calculate flank from the change in state
  return (Flank) (NewState - PrevState);
}


#endif  // _SWITCH_H_