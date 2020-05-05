//
// Created by Justice, Bradley on 4/25/20.
//
#include <File_System_Info.h>

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

    SetBit(Free_Blocks->fbs, 0); //fs info
    SetBit(Free_Blocks->fbs, 1); //root
    SetBit(Free_Blocks->fbs, 2); //fbs1
    SetBit(Free_Blocks->fbs, 3); //fbs2

    SetBit(Free_Blocks2->fbs, 0); //fs info
    SetBit(Free_Blocks2->fbs, 1); //root
    SetBit(Free_Blocks2->fbs, 2); //fbs1
    SetBit(Free_Blocks2->fbs, 3); //fbs2

    //save free block structs to fs struct in memory
    fs->Free_Blocks = Free_Blocks;
    fs->Free_Blocks2 = Free_Blocks2;

    fs->volume_size = volumeSize;

    fs->volume_id = 'C';
    strcpy(fs->volume_name, filename);

    //Before we return, we gotta save it all
    LBAwrite(serialize_fs(fs), 1, 0); //save fs info to LBA 0
    LBAwrite(serialize_de(fs->root), 1, 1); //save root to LBA 1
    LBAwrite(serialize_fbs(fs->Free_Blocks), 1, 2); //save freeblocks1 to LBA 2
    LBAwrite(serialize_fbs(fs->Free_Blocks2), 1, 3); //save freeblocks2 to LBA 3

    return fs;
}

int tryOpen() {
    char *buffer2 = malloc(1000);
    LBAread(buffer2, 1, -1);
    if (strcmp(buffer2, "") == 0) {
        printf("There is nothing here! - must initialize\n");
        return -1;
    } else {
        printf("FOUND SOMETHING!\n"); //return 1 which will trigger read
        return 1;
    }
}

struct File_System_Info *readExistingFs() {
    //read existing system state into memory
    char * buffer = malloc(1000);

    memset(buffer, 0, 1000);
    LBAread(buffer, 1, 0); //read file system object at 0
    struct File_System_Info *preExistingFileSystem = deserialize_fs(buffer);

    memset(buffer, 0, 1000);
    LBAread(buffer, 1, 1); //read root dir_entry at 1
    struct Dir_Entry *pastRoot = deserialize_de(buffer);

    memset(buffer, 0, 1000);
    LBAread(buffer, 1, 2); //read fbs1 at 2
    struct Free_Blocks *freeBlocks1 = deserialize_fbs(buffer);

    memset(buffer, 0, 1000);
    LBAread(buffer, 1, 3); //read fbs2 at 3
    struct Free_Blocks *freeBlocks2 = deserialize_fbs(buffer);

    preExistingFileSystem->root = pastRoot;
    preExistingFileSystem->Free_Blocks = freeBlocks1;
    preExistingFileSystem->Free_Blocks2 = freeBlocks2;

    return preExistingFileSystem;
}

char* serialize_fs(const struct File_System_Info *fs) {
    char * buffer = malloc(
            sizeof(long) + //volume_size
            sizeof(char)*30 + //volume_name
            sizeof(char) + //volume_id
            sizeof(int)//blockSize;
            );

    memcpy(buffer, &fs->volume_size, sizeof(long));
    memcpy(buffer + sizeof(long), &fs->volume_name, strlen(fs->volume_name));
    memcpy(buffer + sizeof(long) + sizeof(char)*30, &fs->volume_id, sizeof(char));
    memcpy(buffer + sizeof(long) + sizeof(char)*31, &fs->blockSize, sizeof(int));

    return buffer;
}

struct File_System_Info *deserialize_fs(char *buffer) {
    struct File_System_Info *result = malloc(sizeof(struct File_System_Info));

    unsigned long vs = 0;
    memcpy(&vs, buffer, sizeof(long));
    result->volume_size = vs;

    memset(&result->volume_name, 0, sizeof(char)*30);
    memcpy(&result->volume_name, buffer + sizeof(long), sizeof(char)*30);

    char c = ' ';
    memcpy(&c, buffer + sizeof(long) + sizeof(char)*30, sizeof(char)*1);
    result->volume_id = c;

    int bs = 0;
    memcpy(&bs, buffer + sizeof(long) + sizeof(char)*31, sizeof(int));
    result->blockSize = bs;

    return result;
}