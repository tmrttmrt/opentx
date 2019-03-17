/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x 
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _PULSES_ESP32_H_
#define _PULSES_ESP32_H_

extern uint8_t s_current_protocol[1];
extern uint8_t s_pulses_paused;

extern volatile uint32_t nextMixerEndTime;

extern uint8_t moduleFlag[NUM_MODULES];

void startPulses();
inline bool pulsesStarted() { return s_current_protocol[0] != 255; }
inline void pausePulses() { s_pulses_paused = true; }
void resumePulses();
void setupPulses();
void DSM2_Init();
void DSM2_Done();

#define SEND_FAILSAFE_NOW()
#define SEND_FAILSAFE_1S()

#endif // _PULSES_ESP32_H_
/*eof*/
