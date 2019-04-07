#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "fs_wrappers.h"

static const char *TAG = "unistd.cpp";
static const char *SL = "/";
static char cpath[CONFIG_FATFS_MAX_LFN+1]="/";

int wr_chdir(const char *path){
    ESP_LOGD(TAG, "wr_chdir(\"%s\"), cpath: \"%s\"",path,cpath);
    if (!path) {
        errno = EFAULT;
        return -1;
    }

    if (!*path) {
        errno = ENOENT;
        return -1;
    }
    
    char np[CONFIG_FATFS_MAX_LFN+1];
    if(*path=='/'){
        strcpy(np,path);
    } else {
        strcpy(np,cpath);
        strcat(np,SL);
        strcat(np,path);
    }
    char bp[CONFIG_FATFS_MAX_LFN+1]="";
    char *pt = strtok(np,SL);
    while(NULL!=pt){
        if('.' == *pt && 0 == *(pt+1)){
            pt = strtok(NULL,SL);
            continue;
        } else if('.' == *pt && '.' == *(pt+1) && 0 == *(pt+2)){
            ssize_t len = strlen(bp);
            while (--len>=0) {
                if (bp[len] == '/') {
                    bp[len] = '\0';
                    break;
                }
            }
            if(len<0){
                errno=EFAULT;
                return -1;
            }
        } else {
            strcat(bp,SL);
            strcat(bp,pt);
        }
        pt = strtok(NULL,SL);
    }
    if(0==*bp){
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

char *wr_getcwd(char *buf, size_t size){
    if(strlen(cpath)<size){
        strcpy(buf,cpath);
        ESP_LOGI(TAG,"getcwd: '%s'",buf);
        return buf;
    } else {
        errno=ERANGE;
        return NULL;
    }
}

WR_DIR *wr_opendir(const char *name){
    WR_DIR *pd=(WR_DIR *)malloc(sizeof(WR_DIR));
    const char *p;
    if(NULL == pd){
        errno=ENOMEM;
        return NULL;
    }
    pd->idx=0;
    if(0 == strcmp(".",name)){
        p=cpath;
    } else {
        p=name;
    }
    if(0 == strcmp(SL,p)){
        pd->isRoot=true;
        return pd;
    }
    pd->isRoot=false;
    pd->pd=opendir(p);
    if(NULL==pd->pd){
        free(pd);
        return(NULL);
    }
    return pd;
}

int wr_closedir(WR_DIR *dirp){
    int ret=0;
    if(!dirp->isRoot){
         ret=closedir(dirp->pd);
    }
    free(dirp);
    return ret;
}

struct dirent *wr_readdir(WR_DIR *dirp){
    if(dirp->isRoot){
        switch (dirp->idx){
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
        switch (dirp->idx){
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