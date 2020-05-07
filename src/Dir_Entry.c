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
    root->file_type = 6; //6 for dir
    root->contentsLocation = 1;
    root->permissions = 644;
    time_t t;   // not a primitive datatype
    time(&t);
    root->date_created = malloc(sizeof(char) * 30);
    root->date_modified = malloc(sizeof(char) * 30);
    strcpy(root->date_created, ctime(&t));
    strcpy(root->date_modified, root->date_created);
    printf("\nThis program has been written at (date and time): %s", ctime(&t));
    root->size = 1;
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
    int size = (sizeof(char) * 30) + //name
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

    memcpy(buffer, &fs->name, strlen(fs->name));
    memcpy(buffer+30, &fs->file_type, sizeof(fs->file_type));
    memcpy(buffer+30+1, &fs->permissions, (sizeof(fs->permissions)));
    memcpy(buffer+30+1+2, fs->date_created, strlen(fs->date_created));
    memcpy(buffer+30+1+2+30, fs->date_modified, strlen(fs->date_modified));
    memcpy(buffer+30+1+2+30+30, &fs->size, (sizeof(long)));
    memcpy(buffer+30+1+2+30+30+(sizeof(long)), &fs->contentsLocation, (sizeof(long)));
    memcpy(buffer+30+1+2+30+30+(sizeof(long)*2), &fs->numFiles, (sizeof(int)));
    memcpy(buffer+30+1+2+30+30+(sizeof(long)*2 + sizeof(int)), fs->fileLBAaddresses, (sizeof(long)*(fs->numFiles)));

    return buffer;
}

struct Dir_Entry *deserialize_de(char *buffer) {
    struct Dir_Entry *result = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));
    memcpy(&result->name, buffer, (sizeof(char) * 30));

    int file_type = 0;
    memcpy(&file_type, (buffer+30), 1);
    result->file_type = file_type;

    int permissions = 0;
    memcpy(&permissions, buffer+30+1, 2);
    result->permissions = permissions;

    result->date_created = malloc(30);
    result->date_modified = malloc(30);
    memcpy(result->date_created, buffer+30+1+2, (sizeof(char) * 30));
    memcpy(result->date_modified, buffer+30+1+2+30, (sizeof(char) * 30));

    unsigned long size = 0;
    memcpy(&size, buffer+30+1+2+30+30, sizeof(long));
    result->size = size;

    unsigned long location = 0;
    memcpy(&location, buffer+30+1+2+30+30+sizeof(long), sizeof(long));
    result->contentsLocation = location;

    int numFiles = 0;
    memcpy(&numFiles, buffer+30+1+2+30+30+sizeof(long)*2, sizeof(int));
    result->numFiles = numFiles;

    unsigned long *fileLBAaddresses = malloc(sizeof(long) * numFiles);
    for (int i = 0; i < numFiles; i++) {
        *(fileLBAaddresses+i) = 0;
    }
    memcpy(fileLBAaddresses, buffer+30+1+2+30+30+(sizeof(long)*2)+sizeof(int), sizeof(long)*numFiles);
    result->fileLBAaddresses = fileLBAaddresses;

    return result;
}