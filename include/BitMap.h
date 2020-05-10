//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_BITMAP_H
#define FILESYSTEM_BITMAP_H

#include <stdbool.h>

void SetBit(int *fbs, int k);

void ClearBit(int *fbs, int k);

bool CheckBit(int *fbs, int k);

#endif //FILESYSTEM_BITMAP_H
