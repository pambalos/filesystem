//
// Created by Justice, Bradley on 4/25/20.
//

#include "BitMap.h"

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