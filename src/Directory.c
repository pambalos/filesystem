//
// Created by Justice, Bradley on 4/16/20.
//

#include <zconf.h>
#include <errno.h>
#include "Directory.h"

struct Dir_Entry *rootinit() {
    struct Dir_Entry *root = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));
    strcpy(root->name, "home");
    root->file_type = dir;

    return root;
    //Set up bin/ for commands maybe...
}

int tryOpen() {
    char *buffer2 = malloc(sizeof(struct Free_Blocks)+2);
    LBAread(buffer2, sizeof(struct Free_Blocks)/BLOCK_SIZE+1, 1);
    struct Free_Blocks *testD = deserialize_fbs(buffer2);
    //serialiazation testing...
    printf("after deserialization\n");
    if ( CheckBit(testD->fbs, 0) ) {
        printf("Bit %d was set !\n", 0);
    } else {
        printf("Bit %d was NOT set !\n", 0);
    }
    if ( CheckBit(testD->fbs, 1) ) {
        printf("Bit %d was set !\n", 1);
    } else {
        printf("Bit %d was NOT set !\n", 1);
    }
}

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
        //int open = tryOpen();

    }
    //On return
    //// 		return value 0 = success;
    ////		return value -1 = file exists but can not open for write
    ////		return value -2 = insufficient space for the volume

    struct File_System_Info *fs = (struct File_System_Info *)malloc(sizeof(struct File_System_Info *));

    //initialize root directory, set root metadata
    struct Dir_Entry *root = rootinit();

    //save root to fs struct
    fs->root = root;

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


    SetBit(Free_Blocks->fbs, 0);
    if ( CheckBit(Free_Blocks->fbs, 0) ) {
        printf("Bit %d was set !\n", 0);
    } else {
        printf("Bit %d was NOT set !\n", 0);
    }
    if ( CheckBit(Free_Blocks->fbs, 1) ) {
        printf("Bit %d was set !\n", 1);
    } else {
        printf("Bit %d was NOT set !\n", 1);
    }

    for (int i = 0; i < (Free_Blocks2->Num_Free_Blocks)/32 + 1; i++) {
        *(Free_Blocks2->fbs+i) = 0;
    }


    //save free block structs to fs struct
    fs->Free_Blocks = Free_Blocks;
    fs->Free_Blocks2 = Free_Blocks2;
    fs->volume_id = 'C';
    strcpy(fs->volume_name, filename);


    char* srl = serialize_fbs(Free_Blocks);

    printf("after deserialization\n");
    struct Free_Blocks *testD = deserialize_fbs(srl);
    if ( CheckBit(testD->fbs, 0) ) {
        printf("Bit %d was set !\n", 0);
    } else {
        printf("Bit %d was NOT set !\n", 0);
    }
    if ( CheckBit(testD->fbs, 1) ) {
        printf("Bit %d was set !\n", 1);
    } else {
        printf("Bit %d was NOT set !\n", 1);
    }
    /*
    int s1 = sizeof(Free_Blocks); //size is 8 bytes

    char *buffer = malloc(BLOCK_SIZE*2);
    char *buffer2 = malloc(sizeof(Free_Blocks)+2);
    memset(buffer, 0, sizeof(Free_Blocks));
    memcpy(buffer, srl, sizeof(Free_Blocks));
    LBAwrite(buffer, 2, 1);
    LBAread(buffer2, 2, 1);

    if (memcmp(buffer, buffer2, sizeof(Free_Blocks)) == 0)
    {
        printf("Read/Write worked\n");
    }
    else
        printf("FAILURE on Write/Read\n");


    struct Free_Blocks *testD = deserialize_fbs(buffer2);
     //serialiazation testing...
    printf("after deserialization\n");
    if ( CheckBit(testD->fbs, 0) ) {
        printf("Bit %d was set !\n", 0);
    } else {
        printf("Bit %d was NOT set !\n", 0);
    }
    if ( CheckBit(testD->fbs, 1) ) {
        printf("Bit %d was set !\n", 1);
    } else {
        printf("Bit %d was NOT set !\n", 1);
    }


    free(buffer);
    free(buffer2);
    */
    return fs;
}

char* serialize_fs(const struct File_System_Info *fs) {

}

char* serialize_de(const struct Dir_Entry *fs) {

}

char* serialize_fbs(struct Free_Blocks *Free_Blocks) {
    char* buffer = malloc(sizeof(int) + sizeof(int) * (Free_Blocks->Num_Free_Blocks/32 + 1));
    memcpy(buffer, &(Free_Blocks->Num_Free_Blocks), sizeof(int));
    memcpy(buffer+sizeof(int), Free_Blocks->fbs, sizeof(int)*(Free_Blocks->Num_Free_Blocks/32 + 1));
    return buffer;
}

struct Free_Blocks* deserialize_fbs(char *buffer) {
    int Num_Free_Blocks;
    memcpy(&Num_Free_Blocks, buffer, sizeof(int));
    int *efbs = (int*)malloc(sizeof(int) * ((Num_Free_Blocks)/32 + 1));
    //Now we need to set all the bits to 0 for free
    //memset(&efbs, 0, (Num_Free_Blocks)/32 + 1);
    memcpy(efbs, (buffer + sizeof(int)), sizeof(int) * (Num_Free_Blocks/32 + 1));
    struct Free_Blocks* res = (struct Free_Blocks*) malloc(sizeof(struct Free_Blocks*) +sizeof(int) * (Num_Free_Blocks/32 + 1));
    res->Num_Free_Blocks = Num_Free_Blocks;
    res->fbs = efbs;
    return res;
}

void SetBit(int *fbs, int k) {
    unsigned int i = k/32;            // i = array index (use: fbs[i])
    unsigned int pos = k%32;          // pos = bit position in fbs[i]

    unsigned int flag = 1;   // flag = 0000.....00001

    flag = flag << pos;      // flag = 0000...010...000   (shifted k positions)

    *(fbs+i) = *(fbs+i) | flag;      // Set the bit at the k-th position in fbs[i]
}

void ClearBit(int *fbs, int k) {
    unsigned int i = k/32;
    unsigned int pos = k%32;

    unsigned int flag = 1;  // flag = 0000.....00001

    flag = flag << pos;     // flag = 0000...010...000   (shifted k positions)
    flag = ~flag;           // flag = 1111...101..111

    *(fbs+i) = *(fbs+i) & flag;     // RESET the bit at the k-th position in A[i]
}

bool CheckBit(int *fbs, int k) {
    unsigned int i = k/32;
    unsigned int pos = k%32;

    unsigned int flag = 1;  // flag = 0000.....00001

    flag = flag << pos;     // flag = 0000...010...000   (shifted k positions)

    if ( *(fbs+i) & flag ) {
        // k-th bit is 1
        return true;
    } else {
        // k-th bit is 0
        return false;
    }
}