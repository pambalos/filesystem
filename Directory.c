//
// Created by Justice, Bradley on 4/16/20.
//

#include "Directory.h"

struct File_System_Info * fsinit(int argc, char *argv[]) {
    //guess that here is where we want to initialize a bunch of stuff
    char * filename;
    uint64_t volumeSize;
    uint64_t blockSize = BLOCK_SIZE;

    if (argc > 3) {
        filename = argv[1];
        volumeSize = atoll (argv[2]);
    }
    int partitionSystemResult = startPartitionSystem (filename, &volumeSize, &blockSize);
    printf("Opened %s, Volume Size: %llu;  BlockSize: %llu; Return %d\n", filename, (ull_t)volumeSize, (ull_t)blockSize, partitionSystemResult);

}
