/*
 *   SwitchKp.h - Library to handle switches and buttons.
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

#ifndef _SWITCHKP_H_
#define _SWITCHKP_H_

#include <Arduino.h>

// Check for these assuptions made in the code
#if LOW != 0x0 || HIGH != 0x1
  #error Unexpected vaules for macros LOW and HIGH
#endif

/*
 *   This library performs debounce for keypads of switches and can provide
    either the switch value or a flank. Does not support multiple simultaneous
    switch presses.
     Switch ids start in 0 and the id for no switch pressed is constant
    SWITCH_NONE.
 */

class SwitchKp
{
public:
  // Default value for the bouncing stabilization time in microseconds
  static const unsigned long DEFAULT_DEBOUNCE_TIME = 250UL;

  // No switch pressed in the keypad
  static const uint8_t SWITCH_NONE = UINT8_MAX;

  // HIGH & LOW macros defined in Arduino.h
  enum Flank_t: int8_t
  {
    FLANK_FALLING = -HIGH,
    FLANK_NONE = LOW,
    FLANK_RISING = HIGH
  };

  // Public methods
  SwitchKp(uint8_t State, unsigned long DebounceTime = DEFAULT_DEBOUNCE_TIME);
  uint8_t updateState(uint8_t ReadSwitch);
  inline Flank_t updateFlank(uint8_t ReadSwitch, uint8_t *pEventSwitch);

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
 *   If a press of a switch is detected while the status reads that a prior
 *  different switch was pressed, it will report a release of the original
 *  switch.
 *   Parameters:
 *   * ReadSwitch: switch for value read in the corresponding pin (LOW: none)
 *   * pEventSwitch: returns the switch for which a press, release or still
 *     pressing has happened. When stable and no switch pressed, it will return
 *     SWITCH_NONE.
 *   Returns: flank produced by this update
 *   * FLANK_FALLING: switch state went from HIGH to LOW
 *   * FLANK_NONE: switch state did not change
 *   * FLANK_RISING: switch state went from LOW to HIGH
 */
inline SwitchKp::Flank_t SwitchKp::updateFlank(uint8_t ReadSwitch,
  uint8_t *pEventSwitch)
{
  uint8_t PrevState, NewState;
  Flank_t Flank;

  // Save current state
  PrevState = _State;

  // Update state
  NewState = updateState(ReadSwitch);

  // Calculate flank from the change in state: the bigger is SWITCH_NONE
  // Also return as event the state that is not SWITCH_NONE, if any.
  if (NewState == PrevState)
  {
    Flank = FLANK_NONE;
    *pEventSwitch = NewState;  // Both are valid
  }
  else if (NewState < PrevState)
  {
    Flank = FLANK_RISING;
    *pEventSwitch = NewState;
  }
  else // if (NewState > PrevState)
  {
    Flank = FLANK_FALLING;
    *pEventSwitch = PrevState;
  }

  return Flank;
}


#endif  // _SWITCHKP_H_