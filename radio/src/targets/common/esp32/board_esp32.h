#ifndef _BOARD_ESP32_H_
#define _BOARD_ESP32_H_
uint16_t getTmr1MHz();
void initADC();
void initKeys();

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


//File system

typedef struct {
    int16_t dummy;
} FIL;

#endif