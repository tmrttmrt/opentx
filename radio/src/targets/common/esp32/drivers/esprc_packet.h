#ifndef ESPRC_PACKET_H
#define ESPRC_PACKET_H

#define ESPNOW_CHANNEL 1
#if !defined(MAX_OUTPUT_CHANNELS)
#define MAX_OUTPUT_CHANNELS  32
#endif
#define BIND_CH 1

enum PacketType_t {
  DATA,
  TELE,
  BIND,
  FSAFE,
  ACK
};

typedef struct {
  PacketType_t type:4;
  uint8_t idx:4;
  uint16_t crc;
  uint16_t ch[MAX_OUTPUT_CHANNELS];
} __attribute__((packed)) TXPacket_t;

typedef struct {
  PacketType_t type:4;
  uint8_t idx:4;
  uint16_t crc;
} __attribute__((packed)) RXPacket_t;

#endif