/*
* Copyright (C) OpenTX
*
* Based on code named
*   th9x - http://code.google.com/p/th9x
*   er9x - http://code.google.com/p/er9x
*   gruvin9x - http://code.google.com/p/gruvin9x
*
* License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

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
#undef DIR

const pm_char * CopyConvertModel_M217(uint8_t i_fileDst, char * path);

const char * const eepromDname = "/flash/eeprom.dir";
const char * const eeGeneralName = "radio.eesp";
static const char *TAG = "eeprom_driver.cpp";
//DRAM_ATTR uint8_t eeprom[EEPROM_SIZE];

char * makeEeFPath(const char *fn)
{
    static char fname[CONFIG_FATFS_MAX_LFN];
    strcpy(fname,eepromDname);
    strcat(fname,"/");
    strcat(fname,fn);
    return(fname);
}

char * makeModPath(uint8_t index)
{
#define MOD_FILE_PREFIX "model-"
    char buff[16]=MOD_FILE_PREFIX;
    itoa(index,buff+sizeof(MOD_FILE_PREFIX)-1,10);
    strcat(buff,".ebin");
    return makeEeFPath(buff);
}

size_t fsWriteModelData(uint8_t index, uint8_t *buff, size_t size)
{
    ESP_LOGI(TAG,"fsWriteModelData: model %d.", index);
    char * fn=makeModPath(index);
    FILE * fp = fopen ( fn, "wb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' for writing.", fn);
        return 0;
    }
    char head[8];
    *(uint32_t*)&head[0] = OTX_FOURCC;
    head[4] = g_eeGeneral.version;
    head[5] = 'M';
    *(uint16_t*)&head[6] = size;
    if(1!=fwrite(head, 8, 1,fp)) {
        fclose(fp);
        ESP_LOGE(TAG,"Failed to write model header to '%s'.", fn);
        return 0;
    }
    if(1!=fwrite(buff, size, 1,fp)) {
        fclose(fp);
        ESP_LOGE(TAG,"Failed to write model data to '%s'.", fn);
        return 0;
    }
    fclose(fp);
    return size;
}

void eeWriteModelData(uint8_t index)
{
    ESP_LOGI(TAG,"Storing model %d.", index);
    fsWriteModelData(index, (uint8_t*)&g_model, sizeof(g_model));
}

void eeWriteGeneral()
{
    ESP_LOGI(TAG,"Storing general eeprom into '%s.'", eeGeneralName );
    char * fn=makeEeFPath(eeGeneralName);
    FILE * fp = fopen ( fn, "wb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s' for writing.", fn);
        return;
    }
        char head[8];
    *(uint32_t*)&head[0] = OTX_FOURCC;
    head[4] = g_eeGeneral.version;
    head[5] = 'M';
    *(uint16_t*)&head[6] = sizeof(g_eeGeneral);
    if(1!=fwrite(head, 8, 1,fp)) {
        fclose(fp);
        ESP_LOGE(TAG,"Failed to write radio settings header to '%s'.", fn);
        return;
    }
    if (1!=fwrite ((uint8_t*)&g_eeGeneral, sizeof(g_eeGeneral), 1,fp) ) {
        ESP_LOGE(TAG,"Failed to write radio settings data to '%s'.", fn);
    }
    fclose(fp);
}


void storageCheck(bool immediately)
{

    if (storageDirtyMsk & EE_GENERAL) {
        ESP_LOGD(TAG,"eeprom write general");
        storageDirtyMsk -= EE_GENERAL;
        eeWriteGeneral();
        if (!immediately) return;
    }

    if (storageDirtyMsk & EE_MODEL) {
        ESP_LOGD(TAG,"eeprom write model");
        storageDirtyMsk = 0;
        eeWriteModelData(g_eeGeneral.currModel);
    }
}

void eeDeleteModel(uint8_t index)
{
    char * fn=makeModPath(index);
    unlink(fn);
}

uint16_t eeModelSize(uint8_t index)
{
    char * fn=makeModPath(index);
    struct stat st;
    if (stat(fn, &st) == 0) {
        return st.st_size;
    }
    return 0;
}

bool eeModelExists(uint8_t id)
{
    if(eeModelSize(id)>0) {
        return true;
    }
    return false;
}

bool eeCopyFile(char * dpath, char * spath)
{
    ESP_LOGI(TAG,"eeCopyFile(%s, %s).", dpath, spath);
    FILE * fps = fopen ( spath, "rb" );
    bool ret=true;
    if (NULL==fps) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' for read.", spath);
        return false;
    }
    FILE * fpd = fopen ( dpath, "wb" );
    if (NULL==fpd) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s' for write.", dpath);
        ESP_LOGE(TAG,"ferr: %d.", ferror(fpd));
        ESP_LOGE(TAG,"feof: %d.", feof(fpd));
        fclose(fps);
        return false;
    }
    int8_t buff[100];
    size_t br;
    while(0<(br=fread(buff, 1,sizeof(buff),fps))) {
        if(br!=fwrite(buff, 1, br, fpd)) {
            ESP_LOGE(TAG,"Failed to write %d bytes to '%s' .", br, dpath);
            ret=false;
            break;
        }
    }
    fclose(fps);
    fclose(fpd);
    return ret;
}

bool eeCopyModel(uint8_t dst, char *spath)
{
    ESP_LOGI(TAG,"eeCopyModel(%d, %s).", dst, spath);
    char * fn=makeModPath(dst);
    return eeCopyFile( fn, spath);

}

bool eeCopyModel(char * dpath, uint8_t src)
{
    ESP_LOGI(TAG,"eeCopyModel(%s, %d).", dpath, src);
    char * fn=makeModPath(src);
    return eeCopyFile(dpath, fn);
}

bool eeCopyModel(uint8_t dst, uint8_t src)
{
    ESP_LOGI(TAG,"eeCopyModel(%d, %d).", dst, src);
    char  fn[CONFIG_FATFS_MAX_LFN];
    strcpy(fn,makeModPath(dst));
    if(eeCopyModel(fn, src)) {
        eeLoadModelHeader(dst, &modelHeaders[dst]);
        return true;
    }
    return false;
}


void eeSwapModels(uint8_t id1, uint8_t id2)
{
    ESP_LOGI(TAG,"eeSwapModels(%d, %d).", id1, id2);
    char * fn1 = strdup(makeModPath(id1));
    char * fn2 = strdup(makeModPath(id2));
    char * fntmp = strdup(makeModPath(MAX_MODELS+1));

    if(eeModelExists(MAX_MODELS+1)) {
        unlink(fntmp);
    }
    if(eeModelExists(id1)) {
        if(0!=rename(fn1,fntmp)) {
            ESP_LOGE(TAG,"Failed to rename '%s' to '%s'.", fn1,fntmp);
        }
    }
    if(eeModelExists(id2)) {
        if(0!=rename(fn2, fn1)) {
            ESP_LOGE(TAG,"Failed to rename '%s' to '%s'.", fn2, fn1);
        } else {
            eeLoadModelHeader(id1, &modelHeaders[id1]);
        }
    }
    if(eeModelExists(MAX_MODELS+1)) {
        if(0!=rename(fntmp,fn2)) {
            ESP_LOGE(TAG,"Failed to rename '%s' to '%s'.", fntmp, fn2);
        } else {
            eeLoadModelHeader(id2, &modelHeaders[id2]);
        }
    }
    free(fn1);
    free(fn2);
    free(fntmp);
}

size_t fsLoadModelData(char *mpath, uint8_t *buff, size_t size,  uint8_t &version)
{
    uint16_t ret=size;
    ESP_LOGI(TAG,"Load model data from '%s'.", mpath );
    FILE * fp = fopen ( mpath, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open '%s'.", mpath);
        return 0;
    }
    char head[8];
    if(1!=fread((uint8_t*)head, 8,1,fp)) {
        fclose(fp);
        ESP_LOGE(TAG,"Failed to read model header from '%s'.", mpath);
        return 0;
    }
    version = (uint8_t)head[4];
    if ((*(uint32_t*)&head[0] != OTX_FOURCC && *(uint32_t*)&head[0] != O9X_FOURCC && *(uint32_t*)&head[0] != OTX_FOURCC_MEGA2560&& *(uint32_t*)&head[0] != O9X_FOURCC_MEGA2560) || (version != FIRST_CONV_EEPROM_VER && version != EEPROM_VER) || head[5] != 'M') {
        fclose(fp);
        ESP_LOGE(TAG,"Incompatible model header from '%s'.", mpath);
        ESP_LOGI(TAG,"OTX_FOURCC:%x, version: %d", *(uint32_t*)&head[0],version);
        return 0;
    }
    ESP_LOGI(TAG,"OTX_FOURCC:%x, version: %d", *(uint32_t*)&head[0], version);
    if(version == FIRST_CONV_EEPROM_VER){ //RLC encoded!
        uint8_t m_zeroes   = 0;
        uint8_t m_bRlc     = 0;
        uint16_t i = 0;
        for( ; 1; ) {
            uint8_t ln = min<uint16_t>(m_zeroes, size-i);
            memclear(&buff[i], ln);
            i        += ln;
            m_zeroes -= ln;
            if (m_zeroes) break;

            ln = min<uint16_t>(m_bRlc, size-i);
            
            uint8_t lr = fread(&buff[i],1, ln,fp);
            if(lr<ln){
                if(ferror(fp)){
                    ret=0;
                }
            }
            i        += lr ;
            m_bRlc   -= lr;
            if(m_bRlc) break;

            if (fread(&m_bRlc, 1,1,fp) !=1){// read how many bytes to read
                if(ferror(fp)){
                    ret=0;
                }
                break; 
            }

            assert(m_bRlc & 0x7f);

            if (m_bRlc&0x80) { // if contains high byte
              m_zeroes  =(m_bRlc>>4) & 0x7;
              m_bRlc    = m_bRlc & 0x0f;
            }
            else if(m_bRlc&0x40) {
              m_zeroes  = m_bRlc & 0x3f;
              m_bRlc    = 0;
            }
        }
        ret=i;
    } else {
        if(1!=fread(buff, size,1,fp)) {
            ret=0;
        }
    }
    fclose(fp);
    if(0==ret){
        ESP_LOGE(TAG,"Failed to read model data from '%s'.", mpath);
    }
    return ret;
}

size_t fsLoadModelData(uint8_t index, uint8_t *buff, size_t size)
{
    ESP_LOGI(TAG,"Load model %d data.", index );
    char * fn=makeModPath(index);
    uint8_t version;
    return fsLoadModelData(fn, buff, size, version);
}

void eeLoadModelName(uint8_t id, char *name)
{
    ESP_LOGI(TAG,"Load model %d.", id );
    memclear(name, sizeof(g_model.header.name));
    if (id < MAX_MODELS) {
        fsLoadModelData(id,(uint8_t *) name, sizeof(g_model.header.name));
    }
}

uint16_t eeLoadModelData(uint8_t index)
{
    memset(&g_model, 0, sizeof(g_model));
    return fsLoadModelData(index,(uint8_t *) &g_model,  sizeof(g_model));
}

void eeLoadModelHeader(uint8_t id, ModelHeader * header)
{
    fsLoadModelData(id,(uint8_t *) header,  sizeof(ModelHeader));
}

bool eeLoadGeneral()
{
    char * fn=makeEeFPath(eeGeneralName);
    ESP_LOGI(TAG,"Load eeGeneral from '%s'.", fn );
    FILE * fp = fopen ( fn, "rb" );
    if (NULL==fp) { /* Check if the file has been opened */
        ESP_LOGE(TAG,"Failed to open ' %s'.", fn);
        generalDefault();
        modelDefault(0);
        return true;
    }
    char head[8];
    if(1!=fread((uint8_t*)head, 8,1,fp)) {
        fclose(fp);
        ESP_LOGE(TAG,"Failed to read radio settings header from '%s'.", fn);
        return 0;
    }
    uint8_t version = (uint8_t)head[4];
    if ((*(uint32_t*)&head[0] != OTX_FOURCC ) || ( version != EEPROM_VER) || head[5] != 'M') {
        fclose(fp);
        ESP_LOGE(TAG,"Incompatible radio settings header from '%s'.", fn);
        ESP_LOGI(TAG,"OTX_FOURCC:%x, version: %d", *(uint32_t*)&head[0],version);
        return 0;
    }
    ESP_LOGI(TAG,"OTX_FOURCC:%x, version: %d", *(uint32_t*)&head[0],version);
    if (fread ((uint8_t*)&g_eeGeneral, sizeof(g_eeGeneral), 1,fp) == 1) {
        if(g_eeGeneral.version == EEPROM_VER) {
            fclose(fp);
            ESP_LOGI(TAG,"EEPROM version %d variant %d", g_eeGeneral.version, g_eeGeneral.variant);
            return true;
        }
    }
    fclose(fp);
    ESP_LOGI(TAG,"EEPROM version %d variant %d", g_eeGeneral.version, g_eeGeneral.variant);
    if (g_eeGeneral.variant != EEPROM_VARIANT) {
        ESP_LOGI(TAG,"EEPROM variant %d instead of %d", g_eeGeneral.variant, EEPROM_VARIANT);
        return false;
    }

#if defined(EEPROM_CONVERSIONS)
    if (g_eeGeneral.version != EEPROM_VER) {
        ESP_LOGI(TAG,"EEPROM version %d instead of %d", g_eeGeneral.version, EEPROM_VER);
        if (!eeConvert()) {
            return false;
        }
    }
    return true;
#else
    ESP_LOGI(TAG,"EEPROM version %d (%d) instead of %d (%d)", g_eeGeneral.version, g_eeGeneral.variant, EEPROM_VER, EEPROM_VARIANT);
    return false;
#endif
}

void clearDir(const char *path)
{
    DIR *dp;
    struct dirent *de;
    char fname[CONFIG_FATFS_MAX_LFN];
    dp=opendir(path);
    if(dp!=0) {
        while(1) {
            de=readdir(dp);
            if (de == NULL) {
                break;
            }
            if (de->d_name[0] == '.' && de->d_name[1] == 0) continue;                           // Ignore . entry
            if (de->d_name[0] == '.' && de->d_name[1] == '.' && de->d_name[2] == 0) continue;   // Ignore .. entry
            strcpy(fname,path);
            strcat(fname,"/");
            strcat(fname,de->d_name);
            unlink(fname);
            ESP_LOGD(TAG,"unlink '%s'.", fname);
        }
        closedir(dp);
    }
}


void storageFormat()
{
    ESP_LOGI(TAG,"storageFormat.");
    if(mkdir(eepromDname,0777) && errno != EEXIST) {
        ESP_LOGE(TAG,"Failed to create directory: '%s'.", eepromDname);
    }
    clearDir(eepromDname);
}

void storageReadRadioSettings()
{
    if (!eeLoadGeneral()) {
        startWiFi(NULL,NULL,NULL);
        isWiFiStarted(3600*1000);
        storageEraseAll(true);
    } else {
        eeLoadModelHeaders();
    }

#if defined(CPUARM)
    for (uint8_t i=0; languagePacks[i]!=NULL; i++) {
        if (!strncmp(g_eeGeneral.ttsLanguage, languagePacks[i]->id, 2)) {
            currentLanguagePackIdx = i;
            currentLanguagePack = languagePacks[i];
        }
    }
#endif
}

#if defined(SDCARD)

const pm_char * eeBackupAll()
{
    char * buf = reusableBuffer.modelsel.mainname;
    char * tail = &buf[sizeof(BACKUP_PATH)];
    if (!sdMounted()) {
        return STR_NO_SDCARD;
    }
    // check and create folder here
    strcpy(buf, STR_BACKUP_PATH);
    ESP_LOGI(TAG, "eeBackupGeneral:buf: '%s'", buf);
    const char * error = sdCheckAndCreateDirectory(buf);
    if (error) {
        return error;
    }
    buf[sizeof(BACKUP_PATH)-1] = '/';
    strcpy(tail,".idx");
    uint8_t idx = 0;
    FILE * fp = fopen ( buf, "rb" );
    if (NULL!=fp) { 
        fread(&idx, 1,sizeof(idx),fp);
        fclose(fp);
        idx++;
    }
    sprintf(tail,"eeprom.dir.%03d",idx);
    error = sdCheckAndCreateDirectory(buf);
    if (error) {
        return error;
    }
    clearDir(buf);
    DIR *dp;
    struct dirent *de;
    char srcfn[CONFIG_FATFS_MAX_LFN];
    strcpy(srcfn,eepromDname);
    strcat(srcfn,"/");
    char * src = srcfn + strlen(srcfn);
    strcat(buf,"/");
    char * dst = buf + strlen(buf);
    dp=opendir(eepromDname);
    if(dp!=0) {
        while(1) {
            de=readdir(dp);
            if (de == NULL) {
                break;
            }
            if (de->d_name[0] == '.' && de->d_name[1] == 0) continue;                           // Ignore . entry
            if (de->d_name[0] == '.' && de->d_name[1] == '.' && de->d_name[2] == 0) continue;   // Ignore .. entry
            strcpy(src,de->d_name);
            strcpy(dst,de->d_name);
            eeCopyFile(buf,srcfn);
        }
        closedir(dp);
        strcpy(tail,".idx");
        fp = fopen ( buf, "wb" );
        if (NULL!=fp) { 
            fwrite(&idx, 1,sizeof(idx),fp);
            fclose(fp);
        } else {
            return STR_SDCARD_ERROR;
        }
        sprintf(tail,"eeprom.dir.%03d",idx-5);
        clearDir(buf);
        unlink(buf);
    }
    return NULL;
}


const pm_char * eeBackupModel(uint8_t i_fileSrc)
{
    char * buf = reusableBuffer.modelsel.mainname;
    ESP_LOGI(TAG, "eeBackupModel:i_fileSrc: %d",(int) i_fileSrc);
    storageCheck(true);
    if (!sdMounted()) {
        return STR_NO_SDCARD;
    }

    // check and create folder here
    strcpy(buf, STR_MODELS_PATH);
    ESP_LOGI(TAG, "eeBackupModel:buf: '%s'", buf);
    const char * error = sdCheckAndCreateDirectory(buf);
    if (error) {
        return error;
    }
    buf[sizeof(MODELS_PATH)-1] = '/';
    strcpy(strcat_modelname(&buf[sizeof(MODELS_PATH)], i_fileSrc), STR_MODELS_EXT);
    ESP_LOGI(TAG, "eeBackupModel:buf: '%s'", buf);
    if(!eeCopyModel(buf, i_fileSrc)) {
        return STR_SDCARD_ERROR;
    }
    return NULL;
}

const pm_char * eeRestoreModel(uint8_t i_fileDst, char *model_name)
{
    char * buf = reusableBuffer.modelsel.mainname;
    storageCheck(true);

    if (!sdMounted()) {
        return STR_NO_SDCARD;
    }

    strcpy(buf, STR_MODELS_PATH);
    buf[sizeof(MODELS_PATH)-1] = '/';
    strcpy(&buf[sizeof(MODELS_PATH)], model_name);
    strcpy(&buf[strlen(buf)], STR_MODELS_EXT);

    uint8_t tmp;
    uint8_t version;
    if(0==fsLoadModelData(buf, &tmp, 1, version)) {
        return STR_INCOMPATIBLE;
    }
    if( FIRST_CONV_EEPROM_VER == version  ){
        return CopyConvertModel_M217(i_fileDst, buf);
    } else {       
        if(0 == eeCopyModel(i_fileDst, buf)) {
            return STR_SDCARD_ERROR;
        }
    }
    eeLoadModelHeader(i_fileDst, &modelHeaders[i_fileDst]);
    return NULL;
}

#endif
