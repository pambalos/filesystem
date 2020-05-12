//
// Created by Justice, Bradley on 4/26/20.
//

#include <File_System_Info.h>
#include "BlockManagementController.h"

//Using the freeblock strucutres located at LBA 2, 3, keep track of what is free, what is taken, make sure both structure reflect the LBA state
//Actually handle saving all objects - needs a simple 'save(file)' for each object type, and read(file)'

int getSizeInBlocks(struct File_System_Info *fs, long size) {
    return size/fs->blockSize;
}

/**
 * A simplified find space and write controller which only looks for contiguous blocks, no support for logical frame mapping
 * @param fs
 * @param buffer
 * @param sizeInBlocks
 * @return
 */
unsigned long findSpace(struct File_System_Info *fs, int sizeInBytes) {
    struct Free_Blocks *Free_Blocks = fs->Free_Blocks; //will be saved at LBA 2
    struct Free_Blocks *Free_Blocks2 = fs->Free_Blocks2; //saved at LBA 3

    bool found = false;
    int count = 0, marker = 0, sizeInBlocks = sizeInBytes/fs->blockSize;

    //find space loop
    for (int i = 0; i < Free_Blocks->Num_Free_Blocks; i++) {
        count = 0;
        marker = i;
        while (CheckBit(Free_Blocks->fbs, i) != true && i < Free_Blocks->Num_Free_Blocks) {
            count++;
            i++;
            if (count == sizeInBlocks) {
                found = true;
                break;
            }
        }
        if (found) break;
    }
    if (!found) {
        printf("Out of space\n");
    }
    return marker;
}