//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_DIR_ENTRY_H
#define FILESYSTEM_DIR_ENTRY_H

enum FileType {
    //0,   1,  2,   3,   4,   5,   6
    txt, zip, sh, exe, pdf, bat, dir
};

struct Dir_Entry {
    char name[30];
    int file_type;
    int permissions;
    char* date_created;
    char* date_modified;
    uint64_t size;
    uint64_t location;
    int numFiles;
    uint64_t *fileLBAaddresses;
};

struct Dir_Entry *rootinit();

char* serialize_de(const struct Dir_Entry *fs);

struct Dir_Entry *deserialize_de(char *buffer);

#endif //FILESYSTEM_DIR_ENTRY_H
