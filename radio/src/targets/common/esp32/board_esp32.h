#ifndef _BOARD_ESP32_H_
#define _BOARD_ESP32_H_

void espLogI(const char * format, ...);

#define eeFlush()
#define DISPLAY_PROGRESS_BAR(x)

bool eepromOpen();
bool eeLoadGeneral();
void eeDeleteModel(uint8_t index);
bool eeCopyModel(uint8_t dst, uint8_t src);
void eeSwapModels(uint8_t id1, uint8_t id2);
uint16_t eeLoadModelData(uint8_t index);
void eeLoadModelName(uint8_t id, char *name);
uint16_t eeModelSize(uint8_t index);


uint16_t getTmr1MHz();
void timer10msInit();
void initADC();
void initKeys();
void eepromInit();
void initAudio();
void initFS();
void setSampleRate(uint32_t frequency);
void audioPlayTask(void * pdata);
void encoderTask(void * pdata);
uint8_t telemetryGetByte(uint8_t * byte);
void telemetryPortInit();
void telemetryTransmitBuffer(uint8_t * data, uint8_t len);
void espLogI(const char * format, ...);
void mixEnterCritical();
void mixExitCritical();
uint16_t audioStackAvailable();
uint16_t per10msStackAvailable();
uint16_t mixerStackAvailable();
uint16_t menusStackAvailable();

void backlightEnable();
void backlightDisable();
bool isBacklightEnabled();


enum Analogs {
  STICK1,
  STICK2,
  STICK3,
  STICK4,
  POT_FIRST,
  POT1 = POT_FIRST,
  POT2,
  POT_LAST = POT2,
  TX_VOLTAGE,
  NUM_ANALOGS
};



#endif