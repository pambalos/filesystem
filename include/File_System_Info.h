//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_FILE_SYSTEM_INFO_H
#define FILESYSTEM_FILE_SYSTEM_INFO_H


#include <stdint.h>
#include <Dir_Entry.h>
#include <fsLow.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Free_Blocks.h>
#include <BitMap.h>

struct File_System_Info { //will be saved at 0
    struct Dir_Entry *root; //saved at LBA 1
    unsigned long volume_size;
    char volume_name[30];
    char volume_id;
    int blockSize;
    struct Free_Blocks *Free_Blocks; //will be saved at LBA 2
    struct Free_Blocks *Free_Blocks2; //saved at LBA 3
};

struct File_System_Info *readExistingFs();

struct File_System_Info * fsinit(int argc, char *argv[]);

struct File_System_Info *newFsInit(char * filename, uint64_t volumeSize, uint64_t blockSize);

int tryOpen();

struct File_System_Info *readExistingFs();

char* serialize_fs(const struct File_System_Info *fs);

struct File_System_Info *deserialize_fs(char *buffer);

#endif //FILESYSTEM_FILE_SYSTEM_INFO_H
