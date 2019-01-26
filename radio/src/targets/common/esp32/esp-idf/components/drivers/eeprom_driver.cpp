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
const char * const eeGeneralName = "eeGeneral.bin";
static const char *TAG = "eeprom_driver.cpp";
//DRAM_ATTR uint8_t eeprom[EEPROM_SIZE];
char fname[CONFIG_FATFS_MAX_LFN];

char * makeEeFPath(const char *fn){
    strcpy(fname,eepromDname);
    strcat(fname,"/");
    strcat(fname,fn);
    return(fname);
}

char * makeModPath(uint8_t index){
#define MOD_FILE_PREFIX "model-"
    char buff[16]=MOD_FILE_PREFIX; 
    itoa(index,buff+sizeof(MOD_FILE_PREFIX)-1,10);
    strcat(buff,".bin");
    return makeEeFPath(buff);
}

void eeWriteModelData(uint8_t index){
    ESP_LOGI(TAG,"Storing model %d.", index);
    char * fn=makeModPath(index);
    FILE * fp = fopen ( fn, "wb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' for writing.", fn);
        return;
    }
    if(1!=fwrite((uint8_t*)&g_model, sizeof(g_model), 1,fp)){
        ESP_LOGE(TAG,"Failed to write model data to '%s' .", fn);
    }
    fclose(fp);
}

void eeWriteGeneral(){
    
    char * fn=makeEeFPath(eeGeneralName);
    FILE * fp = fopen ( fn, "wb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s' for writing.", fn);
        return;
    }
    if (1!=fwrite ((uint8_t*)&g_eeGeneral, sizeof(g_eeGeneral), 1,fp) ) {
        ESP_LOGE(TAG,"Failed to write radio settings data to '%s'.", fn);
    }
    fclose(fp);
}


void storageCheck(bool immediately){
    
    if (storageDirtyMsk & EE_GENERAL) {
        TRACE("eeprom write general");
        storageDirtyMsk -= EE_GENERAL;
        eeWriteGeneral();
        if (!immediately) return;
    }

    if (storageDirtyMsk & EE_MODEL) {
        TRACE("eeprom write model");
        storageDirtyMsk = 0;
        eeWriteModelData(g_eeGeneral.currModel);
    }
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
    ESP_LOGI(TAG,"eeCopyModel(%d, %d).", dst, src);
    char * fn=makeModPath(src);
    FILE * fps = fopen ( fn, "rb" );
    bool ret=true;
    if (NULL==fps) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' for read.", fn);
        return false;
    }
    fn=makeModPath(dst);
    FILE * fpd = fopen ( fn, "wb" );
    if (NULL==fpd) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' for write.", fn);
        ESP_LOGE(TAG,"ferr: %d.", ferror(fpd));
        ESP_LOGE(TAG,"feof: %d.", feof(fpd));
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
    ESP_LOGI(TAG,"eeSwapModels(%d, %d).", id1, id2);
    char * fn1 = strdup(makeModPath(id1));
    char * fn2 = strdup(makeModPath(id2));
    char * fntmp = strdup(makeModPath(MAX_MODELS+1));

    if(eeModelExists(MAX_MODELS+1)){
        unlink(fntmp);
    }
    if(eeModelExists(id1)){
        if(0!=rename(fn1,fntmp)){
            ESP_LOGE(TAG,"Failed to rename '%s' to '%s'.", fn1,fntmp);
        }
    }
    if(eeModelExists(id2)){
        if(0!=rename(fn2, fn1)){
            ESP_LOGE(TAG,"Failed to rename '%s' to '%s'.", fn2, fn1);
        } 
    }
    if(eeModelExists(MAX_MODELS+1)){
        if(0!=rename(fntmp,fn2)){
            ESP_LOGE(TAG,"Failed to rename '%s' to '%s'.", fntmp, fn2);
        }
    }
    free(fn1);
    free(fn2);
    free(fntmp);
}

void eeLoadModelName(uint8_t id, char *name){
    ESP_LOGI(TAG,"Load model %d.", id );
    memclear(name, sizeof(g_model.header.name));
    if (id < MAX_MODELS) {
        char * fn=makeModPath(id);
        FILE * fp = fopen ( fn, "rb" );
        if (NULL==fp) { /* Check if the file has been opened */
            ESP_LOGE(TAG,"Failed to open '%s' .", fn);
            return;
        }
        if(1!=fread((uint8_t*)name, sizeof(g_model.header.name),1,fp)){
            ESP_LOGE(TAG,"Failed to read model name from '%s' .", fn);
        }
        fclose(fp);
    }
}

uint16_t eeLoadModelData(uint8_t index){
    ESP_LOGI(TAG,"Load model %d data.", index );
    memset(&g_model, 0, sizeof(g_model));
    char * fn=makeModPath(index);
    FILE * fp = fopen ( fn, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s'.", fn);
        return 0;
    }
    if(1!=fread((uint8_t*)&g_model, sizeof(g_model),1,fp)){
        ESP_LOGE(TAG,"Failed to read model data from '%s'.", fn);
    }
    fclose(fp);
    return sizeof(g_model);
}

bool eeLoadGeneral(){
    char * fn=makeEeFPath(eeGeneralName);
    FILE * fp = fopen ( fn, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s'.", fn);
        generalDefault();
        modelDefault(0);
        return true;
    }
    if (fread ((uint8_t*)&g_eeGeneral, sizeof(g_eeGeneral), 1,fp) == 1) {
        if(g_eeGeneral.version == EEPROM_VER){
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
    ESP_LOGI(TAG,"storageFormat.");
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
            ESP_LOGI(TAG,"unlink '%s'.", fname);
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

