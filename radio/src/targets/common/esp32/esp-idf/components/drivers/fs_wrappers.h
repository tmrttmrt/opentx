#ifndef _FS_WRAPPERS_H_
#define _FS_WRAPPERS_H_

#define VFS_NATIVE_MOUNT_POINT "/flash"
#define VFS_NATIVE_SDCARD_MOUNT_POINT "/sdcard"

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
WR_DIR *wr_opendir(const char *name);
#endif