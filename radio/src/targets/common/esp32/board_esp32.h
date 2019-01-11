#ifndef _BOARD_ESP32_H_
#define _BOARD_ESP32_H_
uint16_t getTmr1MHz();
void initADC();
void initKeys();
void eepromInit();
void initAudio();
void initFS();

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