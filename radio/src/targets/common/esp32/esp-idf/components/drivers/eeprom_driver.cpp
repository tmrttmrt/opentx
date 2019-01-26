#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

const char * const eepromFname = "/flash/eeprom.bin";
const char * const eepromDname = "/flash/eeprom.dir";
static const char *TAG = "eeprom_driver.cpp";
//DRAM_ATTR uint8_t eeprom[EEPROM_SIZE];
char fname[CONFIG_FATFS_MAX_LFN];

char * makeEeFPath(char *fn){
    strcpy(fname,eepromDname);
    strcat(fname,"/");
    strcat(fname,fn);
    return(fname);
}

char * makeModPath(uint8_t index){
    char buff[10]; 
    itoa(index,buff,10);
    strcat(buff,".bin");
    return makeEeFPath(buff);
}

void storageCheck(bool immediately){
}

void eeDeleteModel(uint8_t index){
    char * fn=makeModPath(index);
    unlink(fn);
}

uint16_t eeModelSize(uint8_t index){
    char * fn=makeModPath(index);
    struct stat st;
    if (stat(fn, &st) == 0) {
        return st.st_size;
    }
    return 0;
}

bool eeModelExists(uint8_t id){
    if(eeModelSize(id)>0){
        return true;
    }
    return false;
}

bool eeCopyModel(uint8_t dst, uint8_t src){
    char * fn=makeModPath(src);
    FILE * fps = fopen ( fn, "rb" );
    bool ret=true;
    if (NULL==fps) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' .", fn);
        return false;
    }
    fn=makeModPath(dst);
    FILE * fpd = fopen ( fn, "wb" );
    if (NULL==fpd) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' .", fn);
        fclose(fps);
        return false;
    }
    int8_t buff[100];
    size_t br;
    while(0<(br=fread(buff, 1 ,sizeof(buff) ,fps))){
        if(br!=fwrite(buff, 1, br, fpd)){
            ESP_LOGE(TAG,"Failed to write %d bytes to '%s' .", br, fn);
            ret=false;
            break;
        }
    }
    fclose(fps);
    fclose(fpd);
    return ret;
}

void eeSwapModels(uint8_t id1, uint8_t id2){
    eeCopyModel(id1, MAX_MODELS+1);
    eeCopyModel(id2, id1);
    eeCopyModel(MAX_MODELS+1, id2);
}

void eeLoadModelName(uint8_t id, char *name){
  memclear(name, sizeof(g_model.header.name));
  if (id < MAX_MODELS) {
    char * fn=makeModPath(id);
    FILE * fp = fopen ( fn, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' .", fn);
    }
    if((sizeof(g_model.header.name))!=fread((uint8_t*)name, sizeof(g_model.header.name),1,fp)){
        ESP_LOGE(TAG,"Failed to read model name from '%s' .", fn);
    }
    fclose(fp);
  }
}

uint16_t eeLoadModelData(uint8_t index){
    memset(&g_model, 0, sizeof(g_model));
    char * fn=makeModPath(index);
    FILE * fp = fopen ( fn, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' .", fn);
    }
    size_t br=fread((uint8_t*)&g_model, sizeof(g_model),1,fp);
    if((sizeof(g_model))!=br){
        ESP_LOGE(TAG,"Failed to read model data from '%s' .", fn);
    }
    fclose(fp);
    return br;
}

bool eeLoadGeneral(){
    const char gn[]="eeGeneral.bin";
    char * fn=makeEeFPath((char *)gn);
    FILE * fp = fopen ( fn, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s' .", fn);
        generalDefault();
        modelDefault(0);
        return true;
    }
    if (fread ((uint8_t*)&g_eeGeneral,1, 3,fp) == 3 && g_eeGeneral.version == EEPROM_VER) {
        if (fread ((uint8_t*)&g_eeGeneral, 1, sizeof(g_eeGeneral),fp) <= sizeof(g_eeGeneral) && g_eeGeneral.variant == EEPROM_VARIANT) {
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
#if defined(EEPROM_CONVERSIONS)
    if (g_eeGeneral.version != EEPROM_VER) {
        TRACE("EEPROM version %d instead of %d", g_eeGeneral.version, EEPROM_VER);
        if (!eeConvert()) {
            return false;
        }
    }
    return true;
#else
    TRACE("EEPROM version %d (%d) instead of %d (%d)", g_eeGeneral.version, g_eeGeneral.variant, EEPROM_VER, EEPROM_VARIANT);
    return false;
#endif
}

void storageFormat()
{
    DIR *dp;
    struct dirent *de;
    char fname[CONFIG_FATFS_MAX_LFN];
    
    if(mkdir(eepromDname,0777) && errno != EEXIST){
        ESP_LOGE(TAG,"Failed to create directory: '%s'.", eepromDname);
    }
    dp=opendir(eepromDname);
    if(dp!=0){
        while(1){
            de=readdir(dp);
            if (de == NULL) {
                break;
            }
            if (de->d_name[0] == '.' && de->d_name[1] == 0) continue;                           // Ignore . entry
            if (de->d_name[0] == '.' && de->d_name[1] == '.' && de->d_name[2] == 0) continue;   // Ignore .. entry
            strcpy(fname,eepromDname);
            strcat(fname,"/");
            strcat(fname,de->d_name);
            unlink(fname);
        }
        closedir(dp);
    }
}

bool eepromOpen(){
    struct stat st;
    const char gn[]="eeGeneral.bin";
    char * fn=makeEeFPath((char *)gn);
    if (stat(fn, &st) == 0) {
        return true;
    }
    return false;
}


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