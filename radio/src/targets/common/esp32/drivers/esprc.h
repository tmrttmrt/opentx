#ifndef ESPRC_H
#define ESPRC_H

#define ESPNOW_CHANNEL 1
#define ESPNOW_QUEUE_SIZE           6
#define MAX_OUTPUT_CHANNELS 16
#define TX_PERIOD_MS 20

enum LinkState_t {
  IDLE,
  SENDERR,
  QUEUED,
  WAITACKN,
  GOTACKN,
  ACKNSENT
};

typedef struct {
  uint8_t idx:4;
  uint8_t type:4;
  uint16_t crc;
  uint16_t ch[MAX_OUTPUT_CHANNELS];
} __attribute__((packed)) TXPacket_t;

enum RXPacketType_t {
  ACK,
  TELE
};

typedef struct {
  uint8_t idx:4;
  RXPacketType_t type:4;
  uint16_t crc;
} __attribute__((packed)) RXPacket_t;

enum EventId_t {
  TX,
  RX
};

typedef struct {
  EventId_t id;
  esp_now_send_status_t status;
  uint8_t mac_addr[ESP_NOW_ETH_ALEN];
  uint8_t *data;
  uint16_t data_len;
} Event_t;

const uint8_t broadcast_mac[ESP_NOW_ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

extern volatile uint32_t packSent;
extern volatile uint32_t packAckn;
extern volatile uint32_t sendPeriod;

esp_err_t initTX();
void moduleSendNextFrame(int16_t *channelOutputs);
#endif