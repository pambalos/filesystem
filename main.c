//
// Created by Justice, Bradley on 4/14/20.
//

#include <stdio.h>
#include "Directory.h"


int main(int argc, char *argv[]) {

    struct File_System_Info *FileSystem = fsinit(argc, argv);

    printf("Hello, World!\n");
    return 0;
}
