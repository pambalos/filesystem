//
// Created by Justice, Bradley on 4/25/20.
//
#include <stdlib.h>
#include <string.h>
#include "Free_Blocks.h"

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
    memcpy(efbs, (buffer + sizeof(int)), sizeof(int) * (Num_Free_Blocks/32 + 1));
    struct Free_Blocks* res = (struct Free_Blocks*) malloc(sizeof(struct Free_Blocks*) +sizeof(int) * (Num_Free_Blocks/32 + 1));
    res->Num_Free_Blocks = Num_Free_Blocks;
    res->fbs = efbs;
    return res;
}