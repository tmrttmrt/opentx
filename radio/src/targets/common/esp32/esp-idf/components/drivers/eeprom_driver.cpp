#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

const char * const eepromFname = "/spiflash/eeprom.bin";
static const char *TAG = "eeprom_driver.cpp";

void IRAM_ATTR eepromReadBlock(uint8_t * buffer, size_t address, size_t size)
{
    ESP_LOGI(TAG,"Reading %d bytes at position %d from '%s'.", size ,address , eepromFname);
    int fp = open ( eepromFname, O_RDONLY );
    if (-1==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s': %d", eepromFname, errno);
        return;
    }
    if (-1==lseek(fp, address, SEEK_SET )) { 
        close(fp);
        ESP_LOGE(TAG,"Failed to seek to position %d bytes in '%s': %d",address, eepromFname, errno);
        return;
    }    
    size_t br = read (fp,(void *)buffer,  size );
    if (br != size ) {
        ESP_LOGE(TAG,"Failed to read %d bytes from '%s': bytes read: %d.", size, eepromFname, br);
        ESP_LOGE(TAG, "error is %d", errno);
    }    
    close(fp);
    ESP_LOGI(TAG,"Read %d bytes at position %d from '%s'.", br ,address , eepromFname);
}

void IRAM_ATTR eepromWriteBlockdis(uint8_t * buffer, size_t address, size_t size)
{
    ESP_LOGI(TAG,"Writing %d bytes at position %d to '%s'.", size ,address , eepromFname);
    FILE * fp=fopen ( eepromFname, "wb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s' .", eepromFname);
        return;
    }
    if (fseek(fp, address, SEEK_SET )) { 
        fclose(fp);
        ESP_LOGE(TAG,"Failed to seek to position %d bytes in '%s'.",address, eepromFname);
        return;
    }    
    size_t bw = fwrite ((void *)buffer, 1 , size, fp);
    if (bw != size ) {       
        ESP_LOGE(TAG,"Failed to write %d bytes to '%s': bytes written: %d.", size, eepromFname, bw);
    }    
    fclose(fp);
    ESP_LOGI(TAG,"Wrote %d bytes at position %d to '%s'.", bw ,address , eepromFname);
}

void IRAM_ATTR eepromReadBlockdis(uint8_t * buffer, size_t address, size_t size)
{
    ESP_LOGI(TAG,"Reading %d bytes at position %d from '%s'.", size ,address , eepromFname);
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
        ESP_LOGE(TAG, "error is %d", errno);
    }    
    fclose(fp);
    ESP_LOGI(TAG,"Read %d bytes at position %d from '%s'.", br ,address , eepromFname);
}

void IRAM_ATTR eepromWriteBlock(uint8_t * buffer, size_t address, size_t size)
{
    ESP_LOGI(TAG,"Writing %d bytes at position %d to '%s'.", size ,address , eepromFname);
    FILE * fp=fopen ( eepromFname, "wb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s' .", eepromFname);
        return;
    }
    if (fseek(fp, address, SEEK_SET )) { 
        fclose(fp);
        ESP_LOGE(TAG,"Failed to seek to position %d bytes in '%s'.",address, eepromFname);
        return;
    }    
    size_t bw = fwrite ((void *)buffer, 1 , size, fp);
    if (bw != size ) {       
        ESP_LOGE(TAG,"Failed to write %d bytes to '%s': bytes written: %d.", size, eepromFname, bw);
    }    
    fclose(fp);
    ESP_LOGI(TAG,"Wrote %d bytes at position %d to '%s'.", bw ,address , eepromFname);
}

uint8_t eepromIsTransferComplete()
{
    return 1;
}


void eepromInit(){
    FILE * fp;
    
    struct stat st;
    if (stat(eepromFname, &st) == 0) {
        ESP_LOGI(TAG,"Opening existing '%s'.",eepromFname);
        fp=fopen ( eepromFname, "wb" );
        if (fp == NULL){
            ESP_LOGE(TAG,"Failed to open '%s'.", eepromFname);
            return;
        }
        ESP_LOGI(TAG,"'%s' opened. Size: %ld bytes.", eepromFname, st.st_size);
        if(st.st_size<EEPROM_SIZE){
            
            char buff[1]={(char)0};
            size_t bw = fwrite ((void *)buff, 1 , EEPROM_SIZE, fp);
            if (bw!=EEPROM_SIZE) { 
                ESP_LOGE(TAG,"Failed to allocate %d bytes for ' %s'.",EEPROM_SIZE, eepromFname);
            }
            fclose(fp);
            if (stat(eepromFname, &st) == 0){
                ESP_LOGI(TAG,"File '%s' expanded to %ld bytes.", eepromFname,  st.st_size);
            }
            return;
        }
        fclose(fp);
    } else {
        ESP_LOGI(TAG,"Creating '%s'.",eepromFname);
        fp=fopen ( eepromFname, "wb+" );
        if (fp == NULL){
            ESP_LOGE(TAG,"Failed to create '%s'.", eepromFname);
            return;
        }
        char buff[1]={(char)0};
        size_t bw = fwrite ((void *)buff, 1 , EEPROM_SIZE, fp);
        if (bw!=EEPROM_SIZE) { 
            ESP_LOGE(TAG,"Failed to allocate %d bytes for ' %s'.",EEPROM_SIZE, eepromFname);
        }
        fclose(fp);
    }
}