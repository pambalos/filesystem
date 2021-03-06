//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_DIR_ENTRY_H
#define FILESYSTEM_DIR_ENTRY_H

enum FileType {
    //0,   1,  2,   3,   4,   5,   6
    txt = 0, zip = 1, sh = 2, exe = 3, pdf = 4, bat = 5, dir = 6
};

struct Dir_Entry {
    long selfAddress;
    long parentAddress;
    char name[30];
    int file_type;
    int permissions;
    char* date_created;
    char* date_modified;
    unsigned long sizeInBlocks; //this size will actually just be num Blocks
    unsigned long contentsLocation; //LBA address contents start at
    int numFiles;
    unsigned long *fileLBAaddresses; // 7, 2, 11, 203, 103
};

struct Dir_Entry *rootinit();

char* serialize_de(const struct Dir_Entry *fs);

struct Dir_Entry *deserialize_de(char *buffer);

#endif //FILESYSTEM_DIR_ENTRY_H
