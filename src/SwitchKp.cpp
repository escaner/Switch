/*
 *   Switch.cpp - Library to handle switches and buttons.
 *
 *   Copyright (C) 2021 Óscar Laborda
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

#include "SwitchKp.h"


/*
 *   Constructor.
 *  Initialized the switch with the State passed and disables the _BounceAway
 *  status.
 *   Parameters:
 *   * State: initial state of the switch
 *   * DebounceTime: Time to assume the bouncing has stabilized, in microseconds
 */
SwitchKp::SwitchKp(uint8_t State, unsigned long DebounceTime):
  _DebounceTime(DebounceTime)
{
  // _LastBounceTime initialization not required (but defaults to 0)
  _BouncingAway = false;  // Disable bounce status
  _State = State;
}


/*
 *   Updates the switch internal status with the latest read value, keeping
 *  track of bounces and returns the (possibly new) switch state.
 *   If a press of a  switch is detected while the status reads that a prior
 *  different switch was pressed, it will report a release of the original
 *  switch.
 *   Parameters:
 *   * ReadSwitch: switch for value read in the corresponding pin (LOW: none)
 *   Returns: current state of the keypad removing bounces
 */
uint8_t SwitchKp::updateState(uint8_t ReadSwitch)
{
  unsigned long Time;

  // Is the read value different from the current state?
  if (ReadSwitch != _State)
  {
    // We are bouncing away
    Time = micros();

    // Special case: going from one switch to another is not valid
    if (ReadSwitch != SWITCH_NONE && _State != SWITCH_NONE)
    {
      // This is not valid, treat it as going from _State switch to SWITCH_NONE
      _BouncingAway = false;
      _State = SWITCH_NONE;
    }
    else  // Normal transition between press and release of a switch
      // Were we already bouncing?
      if (_BouncingAway)
      {
        // Check if we were bouncing long enough (subtract is overflow safe)
        if (Time - _LastBounceTime >= _DebounceTime)
        {
          // Convert this bounce into a change of switch state
          _BouncingAway = false;
          _State = ReadSwitch;
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
