//
// Created by Justice, Bradley on 3/17/20.
//
#ifndef FILEDIRECTORY_DIRECTORY_H
#define FILEDIRECTORY_DIRECTORY_H

#include <stdint.h>

#define BLOCK_SIZE 512

enum FileType {
    txt, zip, sh, exe, pdf, bat
};


struct Dir_Entry {
    char name[30];
    enum FileType file_type;
    unsigned char permissions;
    unsigned long int date_created;
    unsigned long int date_modified;
    unsigned long long size;
};

struct Free_Blocks {
    long Num_Free_Blocks;
    int_fast8_t Free_Blocks[];
};

struct File_System_Info {
    struct Dir_Entry *root;
    struct Dir_Entry **LBA;
    unsigned long int volume_size;
    char volume_name[30];
    unsigned int volume_id;
    struct Free_Blocks *Free_Blocks;
    struct Free_Blocks *Free_Blocks2;
};

#endif //FILEDIRECTORY_DIRECTORY_H
