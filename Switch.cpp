/*
 *   Switch.cpp - Library to handle switches and buttons.
 *   Created by Escaner, 2019.
 */

#include "Switch.h"


/*
 *   Constructor.
 *  Initialized the switch with the State passed and disables the _BounceAway
 *  status.
 *   Parameters:
 *   * State: initial state of the switch
 *   * DebounceTime: Time to assume the bouncing has stabilized, in microseconds
 */
Switch::Switch(uint8_t State, unsigned long DebounceTime):
  _DebounceTime(DebounceTime)
{
  // _LastBounceTime initialization not required (but defaults to 0)
  _BouncingAway = false;  // Disable bounce status
  _State = State;
}


/*
 *   Updates the switch internal status with the latest read value, keeping
 *  track of bounces and returns the (possibly new) switch state.
 *   Parameters:
 *   * ReadValue (LOW, HIGH): value read in the corresponding pin
 *   Returns: state of the switch removing bounces
 *   * LOW
 *   * HIGH
 */
uint8_t Switch::updateState(uint8_t ReadValue)
{
  unsigned long Time;

  // Is the read value different from the current state?
  if (ReadValue != _State)
  {
    // We are bouncing away
    Time = micros();

    // Were we already bouncing?
    if (_BouncingAway)
    {
      // Check if we were bouncing long enough (subtract is overflow safe)
      if (Time - _LastBounceTime >= _DebounceTime)
      {
        // Convert this bounce into a change of switch state
        _BouncingAway = false;
        _State = ReadValue;
      }
    }
    else
    {
      // New bounce, note it with time
      _LastBounceTime = Time;
      _BouncingAway = true;
    }
  }
  else
    // No longer bouncing away if we were
    _BouncingAway = false;

  return _State;
}
