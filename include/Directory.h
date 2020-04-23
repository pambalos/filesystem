//
// Created by Justice, Bradley on 3/17/20.
//
#ifndef FILESYSTEM_DIRECTORY_H
#define FILESYSTEM_DIRECTORY_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsLow.h"

#define BLOCK_SIZE 512

enum FileType {
    txt, zip, sh, exe, pdf, bat, dir
};


struct Dir_Entry {
    char name[30];
    enum FileType file_type;
    unsigned char permissions;
    unsigned long int date_created;
    unsigned long int date_modified;
    unsigned long long size;
    struct Dir_Entry *contents;
};

struct Free_Blocks {
    int Num_Free_Blocks;
    int *fbs;
};

struct File_System_Info {
    struct Dir_Entry *root;
    uint64_t volume_size;
    char volume_name[30];
    char volume_id;
    struct Free_Blocks *Free_Blocks;
    struct Free_Blocks *Free_Blocks2;
};

struct File_System_Info *fsinit(int argc, char *argv[]);

struct Dir_Entry *rootinit();

char* serialize_fs(const struct File_System_Info *fs);

char* serialize_de(const struct Dir_Entry *fs);

char* serialize_fbs(struct Free_Blocks *Free_Blocks);

struct Free_Blocks* deserialize_fbs(char *buffer);

int tryOpen();

/**
 * These methods were implemented by adjusting and using this online example's; http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
 * @param fbs free block bit map pointer
 * @param k position
 */
void SetBit(int *fbs, int k);

void ClearBit(int *fbs, int k);

bool CheckBit(int *fbs, int k);

#endif //FILESYSTEM_DIRECTORY_H
