#ifndef _BOARD_ESP32_H_
#define _BOARD_ESP32_H_
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