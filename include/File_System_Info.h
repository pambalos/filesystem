//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_FILE_SYSTEM_INFO_H
#define FILESYSTEM_FILE_SYSTEM_INFO_H

struct File_System_Info;

struct File_System_Info *readExistingFs();

struct File_System_Info * fsinit(int argc, char *argv[]);

struct File_System_Info *newFsInit();

int tryOpen();

struct File_System_Info *readExistingFs();

char* serialize_fs(const struct File_System_Info *fs);

#endif //FILESYSTEM_FILE_SYSTEM_INFO_H
