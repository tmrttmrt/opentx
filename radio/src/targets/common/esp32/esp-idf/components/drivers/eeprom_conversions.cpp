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

#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

static const char *TAG = "eeprom_driver.cpp";

#undef ESP32_FIELD
#define ESP32_FIELD(x)

#define SWITCHES_WARNING_DATA \
    swarnstate_t  switchWarningState; \
    swarnenable_t switchWarningEnable;
#define MODEL_GVARS_DATA GVarData gvars[MAX_GVARS];
#define TELEMETRY_DATA NOBACKUP(FrSkyTelemetryData frsky);
#define NOBACKUP(...)                __VA_ARGS__
#define MODELDATA_EXTRA
#define CUSTOM_SCREENS_DATA
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)




PACK(struct CustomFunctionDataM217 {
    int8_t  swtch;
    uint8_t func;
    uint8_t mode:2;
    uint8_t param:4;
    uint8_t active:1;
    uint8_t spare:1;
    uint8_t value;
});

PACK(struct ModelDataM217 {
    ModelHeader header;
    TimerData timers[MAX_TIMERS];
    AVR_FIELD(uint8_t   protocol:3)
    ARM_FIELD(uint8_t   telemetryProtocol:3)
    uint8_t   thrTrim:1;            // Enable Throttle Trim
    AVR_FIELD(int8_t    ppmNCH:4)
    ARM_FIELD(uint8_t   noGlobalFunctions:1)
    ESP32_FIELD(uint8_t   noGlobalFunctions:1)
    ARM_FIELD(uint8_t   displayTrims:2)
    ARM_FIELD(uint8_t   ignoreSensorIds:1)
    int8_t    trimInc:3;            // Trim Increments
    uint8_t   disableThrottleWarning:1;
    ARM_FIELD(uint8_t displayChecklist:1)
    AVR_FIELD(uint8_t pulsePol:1)
    uint8_t   extendedLimits:1;
    uint8_t   extendedTrims:1;
    uint8_t   throttleReversed:1;
    AVR_FIELD(int8_t ppmDelay)
    BeepANACenter beepANACenter;
    MixData   mixData[MAX_MIXERS];
    LimitData limitData[MAX_OUTPUT_CHANNELS];
    ExpoData  expoData[MAX_EXPOS];

    CurveData curves[MAX_CURVES];
    int8_t    points[MAX_CURVE_POINTS];

    LogicalSwitchData logicalSw[MAX_LOGICAL_SWITCHES];
    CustomFunctionDataM217 customFn[MAX_SPECIAL_FUNCTIONS];
    SwashRingData swashR;
    FlightModeData flightModeData[MAX_FLIGHT_MODES];

    AVR_FIELD(int8_t ppmFrameLength)     // 0=22.5ms  (10ms-30ms) 0.5ms increments

    NOBACKUP(uint8_t thrTraceSrc);

    SWITCHES_WARNING_DATA

    MODEL_GVARS_DATA

    TELEMETRY_DATA

    MODELDATA_EXTRA

    ARM_FIELD(NOBACKUP(TelemetrySensor telemetrySensors[MAX_TELEMETRY_SENSORS]))

    TARANIS_PCBX9E_FIELD(uint8_t toplcdTimer)

    CUSTOM_SCREENS_DATA

});



const char * ConvertModel_M217_to_219(ModelData & model)
{
    assert(sizeof(ModelDataM217) <= sizeof(ModelData));

    ModelDataM217 *ptr = new ModelDataM217;
    if(ptr){
        ModelDataM217 &oldModel = *ptr;
        memcpy(&oldModel, &model, sizeof(oldModel));
        ModelData &newModel = model;
        memset(&newModel, 0, sizeof(ModelData));

        char name[LEN_MODEL_NAME+1];
        zchar2str(name, oldModel.header.name, LEN_MODEL_NAME);
        ESP_LOGI(TAG,"Model %s conversion from v217 to v219", name);

        newModel.header = oldModel.header;
        for(int i=0; i<MAX_TIMERS;i++){
            newModel.timers[i] = oldModel.timers[i];
        }
        newModel.protocol = newModel.protocol;
        newModel.thrTrim = newModel.thrTrim;
        newModel.ppmNCH = oldModel.ppmNCH;
        newModel.noGlobalFunctions=true;
        newModel.trimInc = oldModel.trimInc;
        newModel.disableThrottleWarning = oldModel.disableThrottleWarning;
        newModel.pulsePol = oldModel.pulsePol;
        newModel.extendedLimits = oldModel.extendedLimits;
        newModel.extendedTrims = oldModel.extendedTrims;
        newModel.throttleReversed = oldModel.throttleReversed;
        newModel.ppmDelay = oldModel.ppmDelay;
        newModel.beepANACenter = oldModel.beepANACenter;
        for(int i=0; i<MAX_MIXERS;i++){
            newModel.mixData[i] = oldModel.mixData[i];
        }
        for(int i=0; i<MAX_OUTPUT_CHANNELS;i++){
            newModel.limitData[i] = oldModel.limitData[i];
        }
        for(int i=0; i<MAX_EXPOS;i++){
            newModel.expoData[i] = oldModel.expoData[i];
        }
        for(int i=0; i<MAX_CURVES;i++){
            newModel.curves[i] = oldModel.curves[i];
        }
        for(int i=0; i<MAX_CURVE_POINTS;i++){
            newModel.points[i] = oldModel.points[i];
        }
        for(int i=0; i<MAX_LOGICAL_SWITCHES;i++){
            newModel.logicalSw[i] = oldModel.logicalSw[i];
        }
        for (int i=0; i<MAX_SPECIAL_FUNCTIONS; i++) {
            newModel.customFn[i].func = oldModel.customFn[i].func + 
            (oldModel.customFn[i].func > FUNC_ADJUST_GVAR ? 1 : 0);
            if(newModel.customFn[i].func <= FUNC_PLAY_SOUND){ //We ignore all functions after FUNC_PLAY_SOUND
                newModel.customFn[i].swtch = oldModel.customFn[i].swtch;
                newModel.customFn[i].mode = oldModel.customFn[i].mode;
                newModel.customFn[i].param = oldModel.customFn[i].param;
                newModel.customFn[i].active = oldModel.customFn[i].active;
                newModel.customFn[i].spare = oldModel.customFn[i].spare;
                newModel.customFn[i].value = oldModel.customFn[i].value;
            } else {
                newModel.customFn[i].func = 0;
            }
        }
        newModel.swashR = oldModel.swashR;
        for (int i=0; i<MAX_FLIGHT_MODES; i++) {
            newModel.flightModeData[i] = oldModel.flightModeData[i];
        }
        newModel.ppmFrameLength = oldModel.ppmFrameLength;
        newModel.thrTraceSrc = oldModel.thrTraceSrc;
        newModel.switchWarningState = oldModel.switchWarningState;
        newModel.switchWarningEnable = oldModel.switchWarningEnable;
        for (int i=0; i<MAX_GVARS; i++) {
            newModel.gvars[i] = oldModel.gvars[i];
        }
        newModel.frsky = oldModel.frsky;
        delete ptr;
        return NULL;
    } else {
        return STR_OUT_OF_MEM;
    }
}

const pm_char * CopyConvertModel_M217(uint8_t i_fileDst, char * path){
    uint8_t version;

    ModelData *md= new ModelData;
    if(NULL == md){
        return STR_OUT_OF_MEM;
    }
    if(0 == fsLoadModelData(path, (uint8_t *)md, sizeof(ModelDataM217),  version)){
        delete md;
        return STR_SDCARD_ERROR;
    } else {
        char const * sret = ConvertModel_M217_to_219(*md);
        if(sret){
            delete md;
            return sret;
        }
        fsWriteModelData(i_fileDst,(uint8_t *) md, sizeof(ModelData));
        eeLoadModelHeader(i_fileDst, &modelHeaders[i_fileDst]);
    }
    delete md;
    return NULL;
}
