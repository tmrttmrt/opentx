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

#ifndef _FS_WRAPPERS_H_
#define _FS_WRAPPERS_H_

#define VFS_NATIVE_MOUNT_POINT "/flash"
#define VFS_NATIVE_SDCARD_MOUNT_POINT "/sdcard"

#include "ff.h"
#include <dirent.h>

#define f_getcwd(lfn, maxlen) wr_getcwd(lfn, maxlen)
#define f_unlink(lfn) unlink(lfn)
#define f_chdir(lfn)  wr_chdir(lfn)
#define f_rename(old, new) rename(old, new)

#if !defined(FS_WRAPPERS)
 #define f_open(ff,path,mode) wr_open(ff,path,mode)
 #define f_opendir(dir, path) wr_opendir(dir, path)
 #define f_puts(s, fp) wr_puts(s, fp)
 #define DIR FF_DIR
#endif

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    bool isRoot;
    uint16_t idx;
    DIR *pd;
    struct dirent de;
} WR_DIR;


struct dirent *wr_readdir(WR_DIR *dirp);
char *wr_getcwd(char *buf, size_t size);
int wr_chdir(const char *path);
int wr_closedir(WR_DIR *dirp);
WR_DIR *otx_opendir(const char *name);
FRESULT wr_open(FIL* fp, const TCHAR* path, BYTE flags);
FRESULT wr_opendir(FF_DIR* dir,TCHAR* path);
int wr_puts (TCHAR* buff, FIL* fp );

#if defined(__cplusplus)
}
#endif

#endif