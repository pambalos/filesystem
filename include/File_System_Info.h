//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_FILE_SYSTEM_INFO_H
#define FILESYSTEM_FILE_SYSTEM_INFO_H

#include "fsLow.h"

struct File_System_Info {
    struct Dir_Entry *root;
    uint64_t volume_size;
    char volume_name[30];
    char volume_id;
    int blockSize;
    struct Free_Blocks *Free_Blocks;
    struct Free_Blocks *Free_Blocks2;
};

struct File_System_Info *readExistingFs();

struct File_System_Info * fsinit(int argc, char *argv[]);

struct File_System_Info *newFsInit();

int tryOpen();

struct File_System_Info *readExistingFs();

char* serialize_fs(const struct File_System_Info *fs);

#endif //FILESYSTEM_FILE_SYSTEM_INFO_H
