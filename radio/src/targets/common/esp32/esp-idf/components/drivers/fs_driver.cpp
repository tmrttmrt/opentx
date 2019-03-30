#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "board_esp32.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"
#define HASASSERT
#include "opentx.h"

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
static const char *TAG = "fs_driver";
// Mount path for the partition
const char *base_path = "/flash";
const char *sd_base_path = SD_PATH;
sdmmc_card_t* card = NULL;

#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_MOSI GPIO_NUM_5
#define PIN_NUM_CLK  GPIO_NUM_18
#define PIN_NUM_CS   GPIO_NUM_4

void initFS(){
    ESP_LOGI(TAG, "Mounting FAT filesystem ...");
    // To mount device we need name of device partition, define base_path
    // and allow format partition in case if it is new one and was not formated before
    esp_vfs_fat_mount_config_t mount_config;
    memset(&mount_config, 0, sizeof(mount_config));
    mount_config.max_files = 5;
    mount_config.format_if_mount_failed = true;
    mount_config.allocation_unit_size = CONFIG_WL_SECTOR_SIZE;
    esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }
}

#if defined(SDCARD)

void mountSDCard(){
    ESP_LOGI(TAG, "Initializing SD card SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot=VSPI_HOST;

    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = PIN_NUM_MISO;
    slot_config.gpio_mosi = PIN_NUM_MOSI;
    slot_config.gpio_sck  = PIN_NUM_CLK;
    slot_config.gpio_cs   = PIN_NUM_CS;
    slot_config.dma_channel=2;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    gpio_set_pull_mode(PIN_NUM_MISO, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLUP_ONLY);

    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s).", esp_err_to_name(ret));
        }
        return;
    }
    sdmmc_card_print_info(stdout, card);
}


void sdInit(void){
}

uint32_t sdMounted(void){
    if(card) return true;
    return false;
}

uint32_t sdIsHC(void){
    if(!card) return false;
    return card->ocr & SD_OCR_SDHC_CAP;
}

uint32_t sdGetNoSectors(){
    FATFS *fs;
    DWORD fre_clust;

    /* Get volume information and free clusters of sdcard */
    auto res = f_getfree(ROOT_PATH, &fre_clust, &fs);
    if (res) {
        return 0;
    }
    return (fs->n_fatent - 2) * fs->csize;
}


uint32_t sdGetSize()
{
    return sdGetNoSectors() *FF_SS_SDCARD;
}

uint32_t sdGetFreeSectors(){
    FATFS *fs;
    DWORD fre_clust;

    /* Get volume information and free clusters of sdcard */
    auto res = f_getfree(ROOT_PATH, &fre_clust, &fs);
    if (res) {
        return 0;
    }
    return fre_clust * fs->csize;
}

uint32_t sdGetSpeed()
{
    if(!card) return false;
    return card->csd.tr_speed;
}

bool sdCardFormat(){
    return false;
}

uint8_t logDelay;

void logsInit(){
}

void logsClose(){
}

void logsWrite(){
}

FRESULT f_getcwd (
	TCHAR* buff,	/* Pointer to the directory path */
	UINT len		/* Size of path */
    ){
        return FR_INT_ERR;
    }
    
FRESULT f_chdir (
	const TCHAR* path	/* Pointer to the directory path */
)
{
        ESP_LOGI(TAG, "f_chdir '%s'", path);
        return FR_INT_ERR;
    }
    
    
const pm_char * eeBackupModel(uint8_t i_fileSrc)
{
    char * buf = reusableBuffer.modelsel.mainname;
    FIL archiveFile;
    UINT written;

    storageCheck(true);

    if (!sdMounted()) {
        return STR_NO_SDCARD;
    }

    // check and create folder here
    strcpy(buf, STR_MODELS_PATH);
    const char * error = sdCheckAndCreateDirectory(buf);
    if (error) {
        return error;
    }

    buf[sizeof(MODELS_PATH)-1] = '/';
    strcpy(strcat_modelname(&buf[sizeof(MODELS_PATH)], i_fileSrc), STR_MODELS_EXT);

    FRESULT result = f_open(&archiveFile, buf, FA_CREATE_ALWAYS | FA_WRITE);
    if (result != FR_OK) {
        return SDCARD_ERROR(result);
    }

    uint16_t size = eeModelSize(i_fileSrc);

    *(uint32_t*)&buf[0] = OTX_FOURCC;
    buf[4] = g_eeGeneral.version;
    buf[5] = 'M';
    *(uint16_t*)&buf[6] = size;

    result = f_write(&archiveFile, buf, 8, &written);
    if (result != FR_OK || written != 8) {
        f_close(&archiveFile);
        return SDCARD_ERROR(result);
    }

    uint8_t *mb = (uint8_t *)malloc(size);
    if(NULL == mb){
        f_close(&archiveFile);
        return STR_OUT_OF_MEM;
    }
    if(0==fsLoadModelData(i_fileSrc, mb,  size)){
        free(mb);
        f_close(&archiveFile);
        return SDCARD_ERROR(FR_DISK_ERR);
    }
    result = f_write(&archiveFile, mb, size, &written);
    if (result != FR_OK || written != size) {
        f_close(&archiveFile);
        free(mb);
        return SDCARD_ERROR(result);
    }
    f_close(&archiveFile);
    free(mb);
    return NULL;
}

const pm_char * eeRestoreModel(uint8_t i_fileDst, char *model_name)
{
    char * buf = reusableBuffer.modelsel.mainname;
    FIL restoreFile;
    UINT read;

    storageCheck(true);

    if (!sdMounted()) {
        return STR_NO_SDCARD;
    }

    strcpy(buf, STR_MODELS_PATH);
    buf[sizeof(MODELS_PATH)-1] = '/';
    strcpy(&buf[sizeof(MODELS_PATH)], model_name);
    strcpy(&buf[strlen(buf)], STR_MODELS_EXT);

    FRESULT result = f_open(&restoreFile, buf, FA_OPEN_EXISTING | FA_READ);
    if (result != FR_OK) {
        return SDCARD_ERROR(result);
    }

    if (f_size(&restoreFile) < 8) {
        f_close(&restoreFile);
        return STR_INCOMPATIBLE;
    }

    result = f_read(&restoreFile, (uint8_t *)buf, 8, &read);
    if (result != FR_OK || read != 8) {
        f_close(&restoreFile);
        return SDCARD_ERROR(result);
    }

    uint8_t version = (uint8_t)buf[4];
    if ((*(uint32_t*)&buf[0] != OTX_FOURCC && *(uint32_t*)&buf[0] != O9X_FOURCC) || version < FIRST_CONV_EEPROM_VER || version > EEPROM_VER || buf[5] != 'M') {
        f_close(&restoreFile);
        return STR_INCOMPATIBLE;
    }

    if (eeModelExists(i_fileDst)) {
        eeDeleteModel(i_fileDst);
    }

    uint16_t size = min<uint16_t>(sizeof(g_model), *(uint16_t*)&buf[6]);

    uint8_t *mb = (uint8_t *)malloc(size);
    if(NULL == mb){
        f_close(&restoreFile);
        return STR_OUT_OF_MEM;
    }
    result = f_read(&restoreFile, mb, size, &read);
    if (result != FR_OK || read != size) {
        f_close(&restoreFile);
        free(mb);
        return SDCARD_ERROR(result);
    }
    if(0==fsWriteModelData(i_fileDst, mb, size)){
        f_close(&restoreFile);
        free(mb);
        return SDCARD_ERROR(FR_DISK_ERR);
    }
    f_close(&restoreFile);
    free(mb);
    eeLoadModelHeader(i_fileDst, &modelHeaders[i_fileDst]);

#if defined(EEPROM_CONVERSIONS)
    if (version < EEPROM_VER) {
        ConvertModel(i_fileDst, version);
        eeLoadModel(g_eeGeneral.currModel);
    }
#endif

    return NULL;
}

#endif

#if false
const char * sdCheckAndCreateDirectory(const char * path)
{
    if(mkdir(path,0777) && errno != EEXIST){
        ESP_LOGE(TAG,"Failed to create directory: '%s'.", path);
        return STR_SDCARD_ERROR;
    }
    return NULL;
}

/*
Wrapper around the f_readdir() function which
also returns ".." entry for sub-dirs. (FatFS 0.12 does
not return ".", ".." dirs anymore)
*/
FRESULT sdReadDir(DIR * dir, FILINFO * fno, bool & firstTime)
{
    FRESULT res;
    if (firstTime && !isCwdAtRoot()) {
        // fake parent directory entry
        strcpy(fno->fname, "..");
        fno->fattrib = AM_DIR;
        res = FR_OK;
    }
    else {
        res = f_readdir(dir, fno);                   /* Read a directory item */
    }
    firstTime = false;
    return res;
}

bool sdListFiles(const char * path, const char * extension, const uint8_t maxlen, const char * selection, uint8_t flags)
{
    static uint16_t lastpopupMenuOffset = 0;
    FILINFO fno;
    DIR dir;
    const char * fnExt;
    uint8_t fnLen, extLen;
    char tmpExt[LEN_FILE_EXTENSION_MAX+1] = "\0";

#if defined(CPUARM)
    popupMenuOffsetType = MENU_OFFSET_EXTERNAL;
#endif

#if defined(CPUARM)
    static uint8_t s_last_flags;

    if (selection) {
        s_last_flags = flags;
        if (!isFilePatternAvailable(path, selection, ((flags & LIST_SD_FILE_EXT) ? NULL : extension))) selection = NULL;
    }
    else {
        flags = s_last_flags;
    }
#endif

    if (popupMenuOffset == 0) {
        lastpopupMenuOffset = 0;
        memset(reusableBuffer.modelsel.menu_bss, 0, sizeof(reusableBuffer.modelsel.menu_bss));
    }
    else if (popupMenuOffset == popupMenuNoItems - MENU_MAX_DISPLAY_LINES) {
        lastpopupMenuOffset = 0xffff;
        memset(reusableBuffer.modelsel.menu_bss, 0, sizeof(reusableBuffer.modelsel.menu_bss));
    }
    else if (popupMenuOffset == lastpopupMenuOffset) {
        // should not happen, only there because of Murphy's law
        return true;
    }
    else if (popupMenuOffset > lastpopupMenuOffset) {
        memmove(reusableBuffer.modelsel.menu_bss[0], reusableBuffer.modelsel.menu_bss[1], (MENU_MAX_DISPLAY_LINES-1)*MENU_LINE_LENGTH);
        memset(reusableBuffer.modelsel.menu_bss[MENU_MAX_DISPLAY_LINES-1], 0xff, MENU_LINE_LENGTH);
    }
    else {
        memmove(reusableBuffer.modelsel.menu_bss[1], reusableBuffer.modelsel.menu_bss[0], (MENU_MAX_DISPLAY_LINES-1)*MENU_LINE_LENGTH);
        memset(reusableBuffer.modelsel.menu_bss[0], 0, MENU_LINE_LENGTH);
    }

    popupMenuNoItems = 0;
    POPUP_MENU_SET_BSS_FLAG();

    FRESULT res = f_opendir(&dir, path);
    if (res == FR_OK) {

        if (flags & LIST_NONE_SD_FILE) {
            popupMenuNoItems++;
            if (selection) {
                lastpopupMenuOffset++;
            }
            else if (popupMenuOffset==0 || popupMenuOffset < lastpopupMenuOffset) {
                char * line = reusableBuffer.modelsel.menu_bss[0];
                memset(line, 0, MENU_LINE_LENGTH);
                strcpy(line, "---");
                popupMenuItems[0] = line;
            }
        }

        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) continue;            /* Skip subfolders */
            if (fno.fattrib & AM_HID) continue;            /* Skip hidden files */
            if (fno.fattrib & AM_SYS) continue;            /* Skip system files */

            fnExt = getFileExtension(fno.fname, 0, 0, &fnLen, &extLen);
            fnLen -= extLen;

            //      TRACE_DEBUG("listSdFiles(%s, %s, %u, %s, %u): fn='%s'; fnExt='%s'; match=%d\n",
            //           path, extension, maxlen, (selection ? selection : "nul"), flags, fno.fname, (fnExt ? fnExt : "nul"), (fnExt && isExtensionMatching(fnExt, extension)));
            // file validation checks
            if (!fnLen || fnLen > maxlen || (                                              // wrong size
                        fnExt && extension && (                                                  // extension-based checks follow...
                            !isExtensionMatching(fnExt, extension) || (                            // wrong extension
                                !(flags & LIST_SD_FILE_EXT) &&                                       // only if we want unique file names...
                                strcasecmp(fnExt, getFileExtension(extension)) &&                    // possible duplicate file name...
                                isFilePatternAvailable(path, fno.fname, extension, true, tmpExt) &&  // find the first file from extensions list...
                                strncasecmp(fnExt, tmpExt, LEN_FILE_EXTENSION_MAX)                   // found file doesn't match, this is a duplicate
                                )
                            )
                        ))
            {
                continue;
            }

            popupMenuNoItems++;

            if (!(flags & LIST_SD_FILE_EXT)) {
                fno.fname[fnLen] = '\0';  // strip extension
            }

            if (popupMenuOffset == 0) {
                if (selection && strncasecmp(fno.fname, selection, maxlen) < 0) {
                    lastpopupMenuOffset++;
                }
                else {
                    for (uint8_t i=0; i<MENU_MAX_DISPLAY_LINES; i++) {
                        char * line = reusableBuffer.modelsel.menu_bss[i];
                        if (line[0] == '\0' || strcasecmp(fno.fname, line) < 0) {
                            if (i < MENU_MAX_DISPLAY_LINES-1) memmove(reusableBuffer.modelsel.menu_bss[i+1], line, sizeof(reusableBuffer.modelsel.menu_bss[i]) * (MENU_MAX_DISPLAY_LINES-1-i));
                            memset(line, 0, MENU_LINE_LENGTH);
                            strcpy(line, fno.fname);
                            break;
                        }
                    }
                }
                for (uint8_t i=0; i<min(popupMenuNoItems, (uint16_t)MENU_MAX_DISPLAY_LINES); i++) {
                    popupMenuItems[i] = reusableBuffer.modelsel.menu_bss[i];
                }

            }
            else if (lastpopupMenuOffset == 0xffff) {
                for (int i=MENU_MAX_DISPLAY_LINES-1; i>=0; i--) {
                    char * line = reusableBuffer.modelsel.menu_bss[i];
                    if (line[0] == '\0' || strcasecmp(fno.fname, line) > 0) {
                        if (i > 0) memmove(reusableBuffer.modelsel.menu_bss[0], reusableBuffer.modelsel.menu_bss[1], sizeof(reusableBuffer.modelsel.menu_bss[i]) * i);
                        memset(line, 0, MENU_LINE_LENGTH);
                        strcpy(line, fno.fname);
                        break;
                    }
                }
                for (uint8_t i=0; i<min(popupMenuNoItems, (uint16_t)MENU_MAX_DISPLAY_LINES); i++) {
                    popupMenuItems[i] = reusableBuffer.modelsel.menu_bss[i];
                }
            }
            else if (popupMenuOffset > lastpopupMenuOffset) {
                if (strcasecmp(fno.fname, reusableBuffer.modelsel.menu_bss[MENU_MAX_DISPLAY_LINES-2]) > 0 && strcasecmp(fno.fname, reusableBuffer.modelsel.menu_bss[MENU_MAX_DISPLAY_LINES-1]) < 0) {
                    memset(reusableBuffer.modelsel.menu_bss[MENU_MAX_DISPLAY_LINES-1], 0, MENU_LINE_LENGTH);
                    strcpy(reusableBuffer.modelsel.menu_bss[MENU_MAX_DISPLAY_LINES-1], fno.fname);
                }
            }
            else {
                if (strcasecmp(fno.fname, reusableBuffer.modelsel.menu_bss[1]) < 0 && strcasecmp(fno.fname, reusableBuffer.modelsel.menu_bss[0]) > 0) {
                    memset(reusableBuffer.modelsel.menu_bss[0], 0, MENU_LINE_LENGTH);
                    strcpy(reusableBuffer.modelsel.menu_bss[0], fno.fname);
                }
            }
        }
        f_closedir(&dir);
    }

    if (popupMenuOffset > 0)
    lastpopupMenuOffset = popupMenuOffset;
    else
    popupMenuOffset = lastpopupMenuOffset;

    return popupMenuNoItems;
}

bool isFilePatternAvailable(const char * path, const char * file, const char * pattern = NULL, bool exclDir = true, char * match = NULL)
{
    uint8_t fplen;
    char fqfp[LEN_FILE_PATH_MAX + _MAX_LFN + 1] = "\0";

    fplen = strlen(path);
    if (fplen > LEN_FILE_PATH_MAX) {
        TRACE_ERROR("isFilePatternAvailable(%s) = error: file path too long.\n", path, file);
        return false;
    }

    strcpy(fqfp, path);
    strcpy(fqfp + fplen, "/");
    strncat(fqfp + (++fplen), file, _MAX_LFN);

    if (pattern == NULL) {
        // no extensions list, just check the filename as-is
        return isFileAvailable(fqfp, exclDir);
    }
    else {
        // extensions list search
        const char *ext;
        uint16_t len;
        uint8_t extlen, fnlen;
        int plen;

        getFileExtension(file, 0, 0, &fnlen, &extlen);
        len = fplen + fnlen - extlen;
        fqfp[len] = '\0';
        ext = getFileExtension(pattern, 0, 0, &fnlen, &extlen);
        plen = (int)fnlen;
        while (plen > 0 && ext) {
            strncat(fqfp + len, ext, extlen);
            if (isFileAvailable(fqfp, exclDir)) {
                if (match != NULL) strncat(&(match[0]='\0'), ext, extlen);
                return true;
            }
            plen -= extlen;
            if (plen > 0) {
                fqfp[len] = '\0';
                ext = getFileExtension(pattern, plen, 0, NULL, &extlen);
            }
        }
    }
    return false;
}
#endif