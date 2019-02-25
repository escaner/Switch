/*
 *   Switch.cpp - Library to handle switches and buttons.
 *
 *   Copyright (C) 2019 Óscar Laborda
 *
 *   This file is part of Switch library.
 *
 *   Switch is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *   Switch is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *  along with Switch.  If not, see <https://www.gnu.org/licenses/>.
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
