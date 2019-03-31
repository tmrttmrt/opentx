#ifndef _BOARD_ESP32_H_
#define _BOARD_ESP32_H_

#define strcpy_P strcpy

void espLogI(const char * format, ...);

#define eeFlush()
#define DISPLAY_PROGRESS_BAR(x)

size_t fsLoadModelData(char *mpath, uint8_t *buff, size_t size);
size_t fsLoadModelData(uint8_t index, uint8_t *buff, size_t size);
size_t fsWriteModelData(uint8_t index, uint8_t *buff, size_t size);
bool eepromOpen();
bool eeLoadGeneral();
void eeDeleteModel(uint8_t index);
bool eeCopyModel(char * dpath, uint8_t src);
bool eeCopyModel(uint8_t dst, char *spath);
bool eeCopyModel(uint8_t dst, uint8_t src);
void eeSwapModels(uint8_t id1, uint8_t id2);
uint16_t eeLoadModelData(uint8_t index);
void eeLoadModelName(uint8_t id, char *name);
uint16_t eeModelSize(uint8_t index);


#if defined(SDCARD)
#define SD_IS_HC()                      (sdIsHC())
#define SD_GET_SPEED()                 (sdGetSpeed())
#define _MAX_LFN      CONFIG_FATFS_MAX_LFN
#define SD_PATH "/sdcard"
#define sdPoll10ms()

void sdInit(void);
uint32_t sdIsHC(void);
uint32_t sdGetSpeed(void);
uint32_t sdMounted(void);
const char * eeBackupModel(uint8_t i_fileSrc);
const char * eeRestoreModel(uint8_t i_fileDst, char *model_name);

#endif



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
void mixEnterCritical();
void mixExitCritical();
uint16_t audioStackAvailable();
uint16_t per10msStackAvailable();
uint16_t mixerStackAvailable();
uint16_t menusStackAvailable();
bool rEncDown(uint8_t mask);
void sendToPulses();
void mountSDCard();



void backlightEnable();
void backlightDisable();
bool isBacklightEnabled();

void initWiFi();
void startWiFi( char *ssid_zchar, char *passwd_zchar, char* ftppass_zchar);
void stopWiFi();
const char* getWiFiStatus();
bool isWiFiStarted();

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