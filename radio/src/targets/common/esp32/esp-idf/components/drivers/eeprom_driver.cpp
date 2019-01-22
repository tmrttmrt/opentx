#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

const char * const eepromFname = "/flash/eeprom.bin";
static const char *TAG = "eeprom_driver.cpp";
DRAM_ATTR uint8_t eeprom[EEPROM_SIZE];

void eepromReadBlock(uint8_t * buffer, size_t address, size_t size)
{
    ESP_LOGI(TAG,"Reading %d bytes at position %d from '%s'.", size ,address , eepromFname);
//    memcpy(buffer,eeprom+address,size);
//    return;
    FILE * fp = fopen ( eepromFname, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s' .", eepromFname);
        return;
    }
    if (fseek(fp, address, SEEK_SET )) { 
        fclose(fp);
        ESP_LOGE(TAG,"Failed to seek to position %d bytes in '%s'.",address, eepromFname);
        return;
    }    
    size_t br = fread ((void *)buffer, 1, size , fp);
    if (br != size ) {
        ESP_LOGE(TAG,"Failed to read %d bytes from '%s': bytes read: %d.", size, eepromFname, br);
//        ESP_LOGE(TAG, "ferror is %d", ferror(fp));
//        ESP_LOGE(TAG, "feof is %d", feof(fp));
    }    
    fclose(fp);
    vTaskDelay(1);
//    ESP_LOGI(TAG,"Read %d bytes at position %d from '%s'.", br ,address , eepromFname);
}

void eepromWriteBlock(uint8_t * buffer, size_t address, size_t size)
{
    ESP_LOGI(TAG,"Writing %d bytes at position %d to '%s'.", size ,address , eepromFname);
//    memcpy(eeprom+address,buffer,size);
//    return;

    int fd = open ( eepromFname, O_WRONLY );
    if (-1 == fd) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s' .", eepromFname);
        return;
    }
    if (-1==lseek(fd, address, SEEK_SET)) { 
        close(fd);
        ESP_LOGE(TAG,"Failed to seek to position %d bytes in '%s'.",address, eepromFname);
        return;
    }    
    size_t bw = write (fd,(void *)buffer,  size );
    if (bw != size ) {       
        ESP_LOGE(TAG,"Failed to write %d bytes to '%s': bytes written: %d.", size, eepromFname, bw);
    }    
    close(fd);
    vTaskDelay(1);
}

uint8_t eepromIsTransferComplete()
{
    return 1;
}


void eepromInit(){
    FILE * fp;
    
    struct stat st;
//    unlink(eepromFname);
//    return;
    if (stat(eepromFname, &st) == 0) {
        ESP_LOGI(TAG,"EEPROM file '%s' exists. Size: %ld",eepromFname,st.st_size);
        if(st.st_size!=EEPROM_SIZE){
            ESP_LOGI(TAG,"'%s' is wrong size: %ld bytes. Erasing ...", eepromFname, st.st_size);
            fp=fopen ( eepromFname, "wb" );
            if (fp == NULL){
                ESP_LOGE(TAG,"Failed to open '%s'.", eepromFname);
                return;
            }
            ESP_LOGI(TAG,"'%s' opened. Size: %ld bytes.", eepromFname, st.st_size);
            int8_t ch=0;
            size_t bw=0;
            for(int i=0;i<EEPROM_SIZE;i++){
                bw += fwrite ((void *)&ch, 1 , 1, fp);
            }
            if (bw!=EEPROM_SIZE) { 
                ESP_LOGE(TAG,"Failed to allocate %d bytes for ' %s'.",EEPROM_SIZE, eepromFname);
            }
            fclose(fp);
            if (stat(eepromFname, &st) == 0){
                ESP_LOGI(TAG,"File '%s' expanded to %ld bytes.", eepromFname,  st.st_size);
            }
            return;
            fclose(fp);
        }
    } else {
        ESP_LOGI(TAG,"Creating '%s'.",eepromFname);
        fp=fopen ( eepromFname, "wb+" );
        if (fp == NULL){
            ESP_LOGE(TAG,"Failed to create '%s'.", eepromFname);
            return;
        }
        int8_t ch=0;
        size_t bw=0;
        for(int i=0;i<EEPROM_SIZE;i++){
            bw += fwrite ((void *)&ch, 1 , 1, fp);
        }
        if (bw!=EEPROM_SIZE) { 
            ESP_LOGE(TAG,"Failed to allocate %d bytes for ' %s'.",EEPROM_SIZE, eepromFname);
        }
        fclose(fp);
    }
}