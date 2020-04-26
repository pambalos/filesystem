//
// Created by Justice, Bradley on 4/25/20.
//

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "Dir_Entry.h"

enum FileType {
    txt, zip, sh, exe, pdf, bat, dir
};

struct Dir_Entry {
    char name[30];
    enum FileType file_type;
    int permissions;
    unsigned long int date_created;
    unsigned long int date_modified;
    unsigned long long size;
    struct Dir_Entry **contents;
    uint64_t location;
};

struct Dir_Entry *rootinit() {
    struct Dir_Entry *root = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));
    strcpy(root->name, "home");
    root->file_type = dir;
    root->location = 1;
    return root;
    //Set up bin/ for commands maybe...
}

char* serialize_de(const struct Dir_Entry *fs) {
    char* buffer = malloc(
            (sizeof(char) * 30) + //name
            (sizeof(int) * 1) + //file_type
            (sizeof(int) * 1) + //permissions
            (sizeof(unsigned long int) )
    );
}