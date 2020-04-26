//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_DIR_ENTRY_H
#define FILESYSTEM_DIR_ENTRY_H

enum FileType;

struct Dir_Entry;

struct Dir_Entry *rootinit();

char* serialize_de(const struct Dir_Entry *fs);

#endif //FILESYSTEM_DIR_ENTRY_H
