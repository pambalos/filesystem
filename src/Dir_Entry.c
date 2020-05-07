//
// Created by Justice, Bradley on 4/25/20.
//

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
#include<time.h>
#include "Dir_Entry.h"


struct Dir_Entry *rootinit() {
    struct Dir_Entry *root = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));

    strcpy(root->name, "home\n");
    root->selfAddress = 1; //root is saved at 1
    root->parentAddress = -1; //
    root->file_type = 6; //6 for dir
    root->contentsLocation = 1;
    root->permissions = 644;
    time_t t;   // not a primitive datatype
    time(&t);
    root->date_created = malloc(sizeof(char) * 30);
    root->date_modified = malloc(sizeof(char) * 30);
    strcpy(root->date_created, ctime(&t));
    strcpy(root->date_modified, root->date_created);
    printf("\nRoot folder initialized at (date and time): %s", ctime(&t));
    root->sizeInBlocks = 1;
    root->numFiles = 0;
    /*
    root->fileLBAaddresses = malloc(sizeof(long)*2);
    *root->fileLBAaddresses = 2;
    *(root->fileLBAaddresses+1) = 3;
     */

    return root;
    //Set up bin/ for commands maybe...
}

char* serialize_de(const struct Dir_Entry *fs) {
    unsigned long size =
            (sizeof(long) * 2) + //selfAddress and parentAddress
            (sizeof(char) * 30) + //name
            (sizeof(enum FileType) * 1) + //file_type
            (sizeof(int) * 1) + //permissions
            (sizeof(char) * 30) + //date_created
            (sizeof(char) * 30) + //date_created
            (sizeof(unsigned long long) * 1) + //size
            (sizeof(int) * 1) + //numFiles
            (sizeof(uint64_t) * 1) + //location
            (sizeof(uint64_t) * fs->numFiles);

    char* buffer = (char *) malloc(size);
    memset(buffer, 0, size);
    unsigned long sizeCounter = 0;

    memcpy(buffer, &fs->selfAddress, sizeof(long)); sizeCounter += sizeof(long);
    memcpy(buffer+sizeCounter, &fs->parentAddress, sizeof(long)); sizeCounter += sizeof(long);
    memcpy(buffer+sizeCounter, fs->name, strlen(fs->name)); sizeCounter += sizeof(char)*30;
    memcpy(buffer+sizeCounter, &fs->file_type, sizeof(fs->file_type)); sizeCounter += sizeof(int);
    memcpy(buffer+sizeCounter, &fs->permissions, (sizeof(fs->permissions))); sizeCounter += sizeof(int);
    memcpy(buffer+sizeCounter, fs->date_created, strlen(fs->date_created)); sizeCounter += sizeof(char)*30;
    memcpy(buffer+sizeCounter, fs->date_modified, strlen(fs->date_modified)); sizeCounter += sizeof(char)*30;
    memcpy(buffer+sizeCounter, &fs->sizeInBlocks, (sizeof(long))); sizeCounter += sizeof(long);
    memcpy(buffer+sizeCounter, &fs->contentsLocation, (sizeof(long))); sizeCounter += sizeof(long);
    memcpy(buffer+sizeCounter, &fs->numFiles, (sizeof(int))); sizeCounter += sizeof(int);
    memcpy(buffer+sizeCounter, &fs->fileLBAaddresses, (sizeof(long)*(fs->numFiles)));

    return buffer;
}

struct Dir_Entry *deserialize_de(char *buffer) {
    struct Dir_Entry *result = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));
    unsigned long sizeCounter = 0;

    long selfAddress = 0;
    memcpy(&selfAddress, buffer, sizeof(long)); sizeCounter += sizeof(long);
    result->selfAddress = selfAddress;

    long parentAddress = 0;
    memcpy(&parentAddress, buffer+sizeCounter, sizeof(long)); sizeCounter += sizeof(long);
    result->parentAddress = parentAddress;

    strcpy(&result->name, buffer+sizeCounter); sizeCounter += (sizeof(char) * 30);

    int file_type = 0;
    memcpy(&file_type, buffer+sizeCounter, sizeof(int)); sizeCounter += sizeof(int);
    result->file_type = file_type;

    int permissions = 0;
    memcpy(&permissions, buffer+sizeCounter, sizeof(int)); sizeCounter += sizeof(int);
    result->permissions = permissions;

    result->date_created = malloc(sizeof(char)*30);
    memcpy(result->date_created, buffer+sizeCounter, (sizeof(char) * 30)); sizeCounter += (sizeof(char) * 30);
    result->date_modified = malloc(sizeof(char)*30);
    memcpy(result->date_modified, buffer+sizeCounter, (sizeof(char) * 30)); sizeCounter += (sizeof(char) * 30);

    unsigned long size = 0;
    memcpy(&size, buffer+sizeCounter, sizeof(long)); sizeCounter += sizeof(long);
    result->sizeInBlocks = size;

    unsigned long location = 0;
    memcpy(&location, buffer+sizeCounter, sizeof(long)); sizeCounter += sizeof(long);
    result->contentsLocation = location;

    int numFiles = 0;
    memcpy(&numFiles, buffer+sizeCounter, sizeof(int)); sizeCounter += sizeof(int);
    result->numFiles = numFiles;

    unsigned long *fileLBAaddresses = malloc(sizeof(long) * numFiles);
    for (int i = 0; i < numFiles; i++) {
        *(fileLBAaddresses+i) = 0;
    }
    memcpy(fileLBAaddresses, buffer+sizeCounter, sizeof(long)*numFiles);
    result->fileLBAaddresses = fileLBAaddresses;

    return result;
}