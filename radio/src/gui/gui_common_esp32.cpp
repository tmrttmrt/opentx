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

#include "opentx.h"

enum SwitchContext
{
  LogicalSwitchesContext,
  ModelCustomFunctionsContext,
  GeneralCustomFunctionsContext,
  TimersContext,
  MixesContext
};

bool isAssignableFunctionAvailable(int function)
{
#if defined(OVERRIDE_CHANNEL_FUNCTION) || defined(GVARS)
  bool modelFunctions = (menuHandlers[menuLevel] == menuModelSpecialFunctions);
#endif

  switch (function) {
    case FUNC_OVERRIDE_CHANNEL:
#if defined(OVERRIDE_CHANNEL_FUNCTION)
      return modelFunctions;
#else
      return false;
#endif
    case FUNC_ADJUST_GVAR:
#if defined(GVARS)
      return modelFunctions;
#else
      return false;
#endif
#if !defined(HAPTIC)
    case FUNC_HAPTIC:
#endif
    case FUNC_RESERVE4:
#if !defined(LUA)
    case FUNC_PLAY_SCRIPT:
#endif
    case FUNC_RESERVE5:
      return false;

    default:
      return true;
  }
}

bool isSwitchAvailable(int swtch, SwitchContext context)
{
  bool negative = false;

  if (swtch < 0) {
    if (swtch == -SWSRC_ON || swtch == -SWSRC_ONE) {
      return false;
    }
    negative = true;
    swtch = -swtch;
  }

  if (swtch >= SWSRC_FIRST_SWITCH && swtch <= SWSRC_LAST_SWITCH) {
    UNUSED(negative);
    return true;
  }

#if NUM_XPOTS > 0
  if (swtch >= SWSRC_FIRST_MULTIPOS_SWITCH && swtch <= SWSRC_LAST_MULTIPOS_SWITCH) {
    int index = (swtch - SWSRC_FIRST_MULTIPOS_SWITCH) / XPOTS_MULTIPOS_COUNT;
    if (IS_POT_MULTIPOS(POT1+index)) {
      StepsCalibData * calib = (StepsCalibData *) &g_eeGeneral.calib[POT1+index];
      return (calib->count >= ((swtch - SWSRC_FIRST_MULTIPOS_SWITCH) % XPOTS_MULTIPOS_COUNT));
    }
    else {
      return false;
    }
  }
#endif

#if defined(PCBSKY9X) && defined(REVX)
  if (swtch == SWSRC_REa) {
    return false;
  }
#endif

  if (swtch >= SWSRC_FIRST_LOGICAL_SWITCH && swtch <= SWSRC_LAST_LOGICAL_SWITCH) {
    if (context == GeneralCustomFunctionsContext) {
      return false;
    }
    else if (context != LogicalSwitchesContext) {
      return isLogicalSwitchAvailable(swtch - SWSRC_FIRST_LOGICAL_SWITCH);
    }
  }

  if (context != ModelCustomFunctionsContext && context != GeneralCustomFunctionsContext && (swtch == SWSRC_ON || swtch == SWSRC_ONE)) {
    return false;
  }

  return true;
}

bool isSwitchAvailableInLogicalSwitches(int swtch)
{
  return isSwitchAvailable(swtch, LogicalSwitchesContext);
}

bool isSwitchAvailableInCustomFunctions(int swtch)
{
  if (menuHandlers[menuLevel] == menuModelSpecialFunctions)
    return isSwitchAvailable(swtch, ModelCustomFunctionsContext);
  else
    return isSwitchAvailable(swtch, GeneralCustomFunctionsContext);
}

bool isSwitchAvailableInMixes(int swtch)
{
  return isSwitchAvailable(swtch, MixesContext);
}


bool isChannelUsed(int channel)
{
  for (int i=0; i<MAX_MIXERS; ++i) {
    MixData *md = mixAddress(i);
    if (md->srcRaw == 0) return false;
    if (md->destCh == channel) return true;
    if (md->destCh > channel) return false;
  }
  return false;
}

bool isSourceAvailable(int source)
{
  if (source < 0)
    return false;

#if defined(LUA_MODEL_SCRIPTS)
  if (source>=MIXSRC_FIRST_LUA && source<=MIXSRC_LAST_LUA) {
    div_t qr = div(source-MIXSRC_FIRST_LUA, MAX_SCRIPT_OUTPUTS);
    return (qr.rem<scriptInputsOutputs[qr.quot].outputsCount);
  }
#elif defined(LUA_INPUTS)
  if (source>=MIXSRC_FIRST_LUA && source<=MIXSRC_LAST_LUA)
    return false;
#endif

  if (source>=MIXSRC_FIRST_POT && source<=MIXSRC_LAST_POT) {
    return IS_POT_SLIDER_AVAILABLE(POT1+source-MIXSRC_FIRST_POT);
  }

#if defined(PCBSKY9X) && defined(REVX)
  if (source == MIXSRC_REa) {
    return false;
  }
#endif

  if (source>=MIXSRC_FIRST_SWITCH && source<=MIXSRC_LAST_SWITCH) {
     return SWITCH_EXISTS(source-MIXSRC_FIRST_SWITCH);
  }

#if !defined(HELI)
  if (source>=MIXSRC_CYC1 && source<=MIXSRC_CYC3)
    return false;
#endif

  if (source>=MIXSRC_FIRST_CH && source<=MIXSRC_LAST_CH) {
    return isChannelUsed(source-MIXSRC_FIRST_CH);
  }

  if (source>=MIXSRC_FIRST_LOGICAL_SWITCH && source<=MIXSRC_LAST_LOGICAL_SWITCH) {
    LogicalSwitchData * cs = lswAddress(source-MIXSRC_FIRST_LOGICAL_SWITCH);
    return (cs->func != LS_FUNC_NONE);
  }

#if !defined(GVARS)
  if (source>=MIXSRC_GVAR1 && source<=MIXSRC_LAST_GVAR)
    return false;
#endif


  if (source>=MIXSRC_FIRST_TELEM && source<=MIXSRC_LAST_TELEM) {
    div_t qr = div(source-MIXSRC_FIRST_TELEM, 3);
    if (qr.rem == 0)
      return isTelemetryFieldAvailable(qr.quot);
    else
      return isTelemetryFieldComparisonAvailable(qr.quot);
  }

  return true;
}

bool isTelemetryFieldAvailable(int index)
{
  return false;
}

bool isSourceAvailableInCustomSwitches(int source)
{
  bool result = isSourceAvailable(source);

#if defined(TELEMETRY_FRSKY)
  if (result && source>=MIXSRC_FIRST_TELEM && source<=MIXSRC_LAST_TELEM) {
    div_t qr = div(source-MIXSRC_FIRST_TELEM, 3);
    result = isTelemetryFieldComparisonAvailable(qr.quot);
  }
#endif

  return result;
}

bool isSourceAvailableInGlobalFunctions(int source)
{
  if (source>=MIXSRC_FIRST_TELEM && source<=MIXSRC_LAST_TELEM) {
    return false;
  }
  return isSourceAvailable(source);
}

bool isSourceAvailableInGlobalResetSpecialFunction(int index)
{
    return false;
}

bool isSourceAvailableInResetSpecialFunction(int index)
{

#if TIMERS < 2
  if (index == FUNC_RESET_TIMER2) {
    return false;
  }
#endif
  return true;
}

bool isLogicalSwitchAvailable(int index)
{
  LogicalSwitchData * lsw = lswAddress(index);
  return (lsw->func != LS_FUNC_NONE);
}

bool isTelemetryFieldComparisonAvailable(int index){
    return false;
}