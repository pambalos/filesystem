//
// Created by Justice, Bradley on 4/16/20.
//

#include "Directory.h"

struct File_System_Info * fsinit(int argc, char *argv[]) {
    //guess that here is where we want to initialize a bunch of stuff
    char *filename;
    uint64_t volumeSize;
    uint64_t blockSize;
    int retVal;

    if (argc > 3)
    {
        filename = argv[1];
        volumeSize = atoll(argv[2]);
        blockSize = atoll(argv[3]);
    }

    retVal = startPartitionSystem(filename, &volumeSize, &blockSize);
    printf("Opened %s, Volume Size: %llu;  BlockSize: %llu; Return %d\n", filename, (ull_t)volumeSize, (ull_t)blockSize, retVal);
    struct File_System_Info *fs = (struct File_System_Info *)malloc(sizeof(struct File_System_Info *));



    /*
    char *buf = malloc(blockSize * 2);
    char *buf2 = malloc(blockSize * 2);
    memset(buf, 0, blockSize * 2); // setting all values in buf to 0
    strcpy(buf, "Now is the time for all good people to come to the aid of their countrymen\n");
    strcpy(&buf[blockSize + 10], "Four score and seven years ago our fathers brought forth onto this continent a new nation\n");
    LBAwrite(buf, 2, 0);
    LBAwrite(buf, 2, 3);
    LBAread(buf2, 2, 0);
    if (memcmp(buf, buf2, blockSize * 2) == 0)
    {
        printf("Read/Write worked\n");
    }
    else
        printf("FAILURE on Write/Read\n");

    free(buf);
    free(buf2);
*/
    return fs;
}
