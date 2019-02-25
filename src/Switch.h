/*
 *   Switch.h - Library to handle switches and buttons.
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

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include <Arduino.h>

// Check for these assuptions made in the code
#if LOW != 0x0 || HIGH != 0x1
  #error Unexpected vaules for macros LOW and HIGH
#endif

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