//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_FREE_BLOCKS_H
#define FILESYSTEM_FREE_BLOCKS_H

struct Free_Blocks {
    int Num_Free_Blocks;
    int *fbs;
};

char* serialize_fbs(struct Free_Blocks *Free_Blocks);

struct Free_Blocks* deserialize_fbs(char *buffer);

#endif //FILESYSTEM_FREE_BLOCKS_H
