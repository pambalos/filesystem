//
// Created by Justice, Bradley on 4/25/20.
//
#include <File_System_Info.h>
#include <stdint.h>
#include <Dir_Entry.h>
#include <fsLow.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Free_Blocks.h>
#include <BitMap.h>

struct File_System_Info {
    struct Dir_Entry *root;
    uint64_t volume_size;
    char volume_name[30];
    char volume_id;
    int blockSize;
    struct Free_Blocks *Free_Blocks;
    struct Free_Blocks *Free_Blocks2;
};

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
    if (retVal == 0) {
        //if successfully opened, try to get the root data from LBA 1...
        int open = tryOpen();
        if (open == -1) {
            //initialize everything because nothing existed
            return newFsInit(filename, volumeSize, blockSize);
        } else {
            //something was found
            return readExistingFs();
        }
    } else {
        printf("Something is broken!\nEither:\n\tfile exists but can not open for write\n\tOR\n\tinsufficient space for the volume\n");
        return NULL;
    }

    //On return
    //// 		return value 0 = success;
    ////		return value -1 = file exists but can not open for write
    ////		return value -2 = insufficient space for the volume
}

struct File_System_Info *newFsInit(char * filename, uint64_t volumeSize, uint64_t blockSize) {
    struct File_System_Info *fs = (struct File_System_Info *)malloc(sizeof(struct File_System_Info *));

    //initialize root directory, set root metadata
    struct Dir_Entry *root = rootinit();

    //save root to fs struct
    fs->root = root;
    fs->blockSize = blockSize;

    //initializing file system info, free block structures and trackers
    struct Free_Blocks *Free_Blocks = (struct Free_Blocks *)malloc(sizeof(struct Free_Blocks *) + sizeof(int) * (volumeSize / blockSize + 1));
    struct Free_Blocks *Free_Blocks2 = (struct Free_Blocks *)malloc(sizeof(struct Free_Blocks *) + sizeof(int) * (volumeSize / blockSize + 1));

    Free_Blocks->Num_Free_Blocks = volumeSize / blockSize;

    Free_Blocks2->Num_Free_Blocks = volumeSize / blockSize;

    //fbs are free block array bit maps. http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html has implementation details
    Free_Blocks->fbs = (int *)malloc(sizeof(int) * ((volumeSize / blockSize)/32 + 1)); // Take a look at link^ to understand why its /32
    Free_Blocks2->fbs = (int *)malloc(sizeof(int) * ((volumeSize / blockSize)/32 + 1));

    //Now we need to set all the bits to 0 for free
    for (int i = 0; i < (Free_Blocks->Num_Free_Blocks)/32 + 1; i++) {
        *(Free_Blocks->fbs+i) = 0;
    }
    //SetBit 0 to taken for volume control block
    SetBit(Free_Blocks->fbs, 0);

    for (int i = 0; i < (Free_Blocks2->Num_Free_Blocks)/32 + 1; i++) {
        *(Free_Blocks2->fbs+i) = 0;
    }
    SetBit(Free_Blocks->fbs, 0);

    //save free block structs to fs struct
    fs->Free_Blocks = Free_Blocks;
    fs->Free_Blocks2 = Free_Blocks2;
    fs->volume_id = 'C';
    strcpy(fs->volume_name, filename);

    return fs;
}

int tryOpen() {
    char *buffer2 = malloc(1096);
    LBAread(buffer2, 1, 0);
    if (strcmp(buffer2, "") == 0) {
        printf("There is nothing here! - must initialize\n");
        return -1;
    } else {
        printf("FOUND SOMETHING!\n");
        return 1;
    }
}

struct File_System_Info *readExistingFs() {
    //currently we can read a Free_Blocks, need to do Dir_Entry then File_System_Info

}

char* serialize_fs(const struct File_System_Info *fs) {

}