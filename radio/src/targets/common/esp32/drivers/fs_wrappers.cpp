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


#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "sdkconfig.h"
#define FS_WRAPPERS
#include "opentx.h"

static const char *TAG = "fs_wrappers";
static const char *SL = "/";
static char cpath[CONFIG_FATFS_MAX_LFN+1]="/";



int wr_chdir(const char *path)
{
    ESP_LOGI(TAG, "wr_chdir(\"%s\"), cpath: \"%s\"",path,cpath);
    if (!path) {
        errno = EFAULT;
        return -1;
    }

    if (!*path) {
        errno = ENOENT;
        return -1;
    }

    char np[CONFIG_FATFS_MAX_LFN+1];
    if(*path=='/') {
        strcpy(np,path);
    } else {
        strcpy(np,cpath);
        strcat(np,SL);
        strcat(np,path);
    }
    char bp[CONFIG_FATFS_MAX_LFN+1]="";
    char *pt = strtok(np,SL);
    while(NULL!=pt) {
        if('.' == *pt && 0 == *(pt+1)) {
            pt = strtok(NULL,SL);
            continue;
        } else if('.' == *pt && '.' == *(pt+1) && 0 == *(pt+2)) {
            ssize_t len = strlen(bp);
            while (--len>=0) {
                if (bp[len] == '/') {
                    bp[len] = '\0';
                    break;
                }
            }
            if(len<0) {
                errno=EFAULT;
                return -1;
            }
        } else {
            strcat(bp,SL);
            strcat(bp,pt);
        }
        pt = strtok(NULL,SL);
    }
    if(0==*bp) {
        strcpy(cpath,SL);
        ESP_LOGD(TAG,"cpath: '%s'",cpath);
        return 0;
    }
    struct stat statb;
    if (stat(bp, &statb) || !S_ISDIR(statb.st_mode)) {
        ESP_LOGE(TAG,"stat on '%s' failed: errno: %d",bp,errno);
        errno = ENOTDIR;
        return -1;
    }
//    strcat(bp,SL);
    strcpy(cpath,bp);
    ESP_LOGD(TAG,"cpath: '%s'",cpath);
    return 0;
}

char *wr_getcwd(char *buf, size_t size)
{
    if(strlen(cpath)<size) {
        strcpy(buf,cpath);
        ESP_LOGI(TAG,"getcwd: '%s'",buf);
        return buf;
    } else {
        errno=ERANGE;
        return NULL;
    }
}

WR_DIR *otx_opendir(const char *name)
{
    WR_DIR *pd=(WR_DIR *)malloc(sizeof(WR_DIR));
    const char *p;
    if(NULL == pd) {
        errno=ENOMEM;
        return NULL;
    }
    pd->idx=0;
    if(0 == strcmp(".",name)) {
        p=cpath;
    } else {
        p=name;
    }
    if(0 == strcmp(SL,p)) {
        pd->isRoot=true;
        return pd;
    }
    pd->isRoot=false;
    pd->pd=opendir(p);
    if(NULL==pd->pd) {
        free(pd);
        return(NULL);
    }
    return pd;
}

int wr_closedir(WR_DIR *dirp)
{
    int ret=0;
    if(!dirp->isRoot) {
        ret=closedir(dirp->pd);
    }
    free(dirp);
    return ret;
}

struct dirent *wr_readdir(WR_DIR *dirp)
{
    if(dirp->isRoot) {
        switch (dirp->idx) {
        case 0:
            strcpy(dirp->de.d_name,VFS_NATIVE_MOUNT_POINT+1);
            dirp->de.d_type=DT_DIR;
            dirp->idx=1;
            return &dirp->de;
            break;
        case 1:
            strcpy(dirp->de.d_name,VFS_NATIVE_SDCARD_MOUNT_POINT+1);
            dirp->de.d_type=DT_DIR;
            dirp->idx=2;
            return &dirp->de;
            break;
        default:
            errno = 0;
            return NULL;
            break;
        }
    } else {
        switch (dirp->idx) {
        case 0:
            strcpy(dirp->de.d_name,"..");
            dirp->de.d_type=DT_DIR;
            dirp->idx=1;
            return &dirp->de;
            break;
        default:
            return readdir(dirp->pd);
            break;
        }
    }
    return NULL;
}

FRESULT wr_open(FIL* fp, const TCHAR* path, BYTE flags){
  FRESULT fr=FR_INT_ERR;
  ESP_LOGI(TAG,"wr_open: path:%s\n",path);
  if(0==memcmp(path,SD_PATH,sizeof(SD_PATH)-1)){
    char mpath[CONFIG_FATFS_MAX_LFN+1];
    strcpy(mpath,SDdrv);
    strcat(mpath,path+sizeof(SD_PATH)-1);
    ESP_LOGI(TAG,"wr_open: mpath:%s\n",mpath);
    fr=f_open(fp,mpath,flags);
  }
  return fr;
}

FRESULT wr_opendir(FF_DIR* dir,TCHAR* path){
  FRESULT fr=FR_INT_ERR;
  ESP_LOGI(TAG,"wr_open: path:%s\n",path);
  if(0==memcmp(path,SD_PATH,sizeof(SD_PATH)-1)){
    char mpath[CONFIG_FATFS_MAX_LFN+1];
    strcpy(mpath,SDdrv);
    strcat(mpath,path+sizeof(SD_PATH)-1);
    ESP_LOGI(TAG,"wr_openndir: mpath:%s\n",mpath);
    fr=f_opendir(dir,mpath);
  }
  return fr;
}

int wr_puts (TCHAR* buff, FIL* fp){
  UINT bw;
  UINT btw=strlen(buff);
  if(FR_OK==f_write(fp, buff, btw ,&bw)) 
    return bw==btw?bw:EOF;
  return EOF;
}

