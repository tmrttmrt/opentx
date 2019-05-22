#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "esp_now.h"
#include "esp_task.h"
#include "rom/ets_sys.h"
#include "rom/crc.h"
#include "esprc.h"
#include "opentx.h"

#define EVT_QUEUE_SIZE 

static const char *TAG = "tx.cpp";
static xQueueHandle evtQueue;
static esp_now_peer_info_t rxPeer;
static uint8_t rx_mac_addr[ESP_NOW_ETH_ALEN] = { 0x84, 0x0d, 0x8e, 0x81, 0xe7, 0x4a };
static DRAM_ATTR int16_t locChannelOutputs[MAX_OUTPUT_CHANNELS] = {0};
static QueueHandle_t xPulsesQueue;
static volatile TXPacket_t packet;
static volatile LinkState_t linkState = IDLE;
uint32_t volatile packSent = 0;
uint32_t volatile packAckn = 0;
uint32_t volatile sendPeriod = 0;

void espnow_packet_prepare()
{
  packet.idx++;
  packet.crc = 0;
  xQueueReceive( xPulsesQueue, locChannelOutputs,0);
  memcpy((void *)packet.ch,locChannelOutputs,sizeof(packet.ch));
  packet.crc = crc16_le(0, (uint8_t const *) &packet, sizeof(packet));
}


static void tx_task(void *pvParameter)
{
  Event_t evt;
  vTaskDelay(5000 / portTICK_RATE_MS);
  while (true) {
    if(xQueueReceive(evtQueue, &evt, TX_PERIOD_MS/portTICK_RATE_MS) == pdTRUE) {
      switch (evt.id) {
        case TX:
            if( ESP_NOW_SEND_SUCCESS == evt.status){
              linkState = WAITACKN;
              packSent++;
            } 
            else {
              linkState = SENDERR;
            }
          break;
        case RX:
            if (!memcmp(evt.mac_addr,rx_mac_addr, sizeof(ESP_NOW_ETH_ALEN))) {
              if ( sizeof(RXPacket_t) == evt.data_len ){
                RXPacket_t *rp = (RXPacket_t *) evt.data;
                if (rp->idx == packet.idx && rp->crc == packet.crc){
                  linkState = GOTACKN;
                  packAckn++;
                }
                else {
                  ESP_LOGE(TAG, "Ack failed: idx: %d vs %d, crc: %d vs %d, ", rp->idx , packet.idx, rp->crc , packet.crc);
                } 
              }
            }
            else {
              ESP_LOGE(TAG, "Wrong packet MAC: " MACSTR, MAC2STR(evt.mac_addr));
            }
            free(evt.data);
          break;
      }
    } 
    else {
      static uint64_t last_send_time = 0;
      uint64_t curr_time = esp_timer_get_time();
      sendPeriod = (curr_time - last_send_time)/1000;
      last_send_time = curr_time;
      if( linkState != QUEUED){
        espnow_packet_prepare();
        if (esp_now_send(rx_mac_addr, (const uint8_t *) &packet, sizeof(packet)) != ESP_OK) {
          ESP_LOGE(TAG, "Send txPacket error");
        } 
        else {
          linkState = QUEUED;
        }
      }
    }
  }
}

static void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Event_t evt;
  
  if (mac_addr == NULL) {
    ESP_LOGE(TAG, "Send cb arg error");
    return;
  }
  
  evt.id = TX;
  evt.status = status;
  if (xQueueSend(evtQueue, &evt, portMAX_DELAY) != pdTRUE) {
    ESP_LOGE(TAG, "TX: Send queue failed.");
  }
}

static void recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    Event_t evt;

    if (mac_addr == NULL || data == NULL || len <= 0) {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    evt.id = RX;
    memcpy(evt.mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
    evt.data = (uint8_t *)malloc(len);
    if (evt.data == NULL) {
        ESP_LOGE(TAG, "Malloc receive data fail");
        return;
    }
    memcpy(evt.data, data, len);
    evt.data_len = len;
    if (xQueueSend(evtQueue, &evt, portMAX_DELAY) != pdTRUE) {
        ESP_LOGW(TAG, "RX: Send queue failed.");
        free(evt.data);
    }
}

esp_err_t initTX(){
  evtQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(Event_t));
  if (evtQueue == NULL) {
    ESP_LOGE(TAG, "Create queue failed.");
    return ESP_FAIL;
  }
  
  xPulsesQueue=xQueueCreate( 1, sizeof( locChannelOutputs ) );
  if(NULL == xPulsesQueue){
      ESP_LOGE(TAG, "Failed to create queue: xPulsesQueue!");
  }

  
  ESP_ERROR_CHECK( esp_now_init() );
  ESP_ERROR_CHECK( esp_now_register_send_cb(send_cb) );
  ESP_ERROR_CHECK( esp_now_register_recv_cb(recv_cb) );
  
  rxPeer.channel = ESPNOW_CHANNEL;
  rxPeer.ifidx = ESP_IF_WIFI_AP;
  rxPeer.encrypt = false;
  memcpy(rxPeer.peer_addr, rx_mac_addr, ESP_NOW_ETH_ALEN);
  ESP_ERROR_CHECK( esp_now_add_peer(&rxPeer) );
  
  xTaskCreate(tx_task, "tx_task", 2048, NULL, ESP_TASK_PRIO_MAX-6, NULL);

  return ESP_OK;
  
}

void moduleSendNextFrame(int16_t *channelOutputs)
{
  xQueueOverwrite( xPulsesQueue, channelOutputs );
}

void intmoduleSendNextFrame()
{
  switch (moduleState[INTERNAL_MODULE].protocol) {
    default:
      break;
  }
}