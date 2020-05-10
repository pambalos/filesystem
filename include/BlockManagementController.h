//
// Created by Justice, Bradley on 4/26/20.
//

#ifndef FILESYSTEM_BLOCKMANAGEMENTCONTROLLER_H
#define FILESYSTEM_BLOCKMANAGEMENTCONTROLLER_H

int getSizeInBlocks(struct File_System_Info *fs, long size);

unsigned long findSpace(struct File_System_Info *fs, int sizeInBytes);

#endif //FILESYSTEM_BLOCKMANAGEMENTCONTROLLER_H
