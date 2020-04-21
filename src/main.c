#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <fsLow.h>
#include <Directory.h>

int main(int argc, char *argv[]) {

    struct File_System_Info *fs = fsinit(argc, argv);

    closePartitionSystem();
    return 0;
}


/*
    int A[10];
    int i;

    for ( i = 0; i < 10; i++ )
        A[i] = 0;                    // Clear the bit array

    printf("Set bit poistions 100, 200 and 300\n");
    SetBit( A, 100 );               // Set 3 bits
    SetBit( A, 200 );
    SetBit( A, 300 );


    // Check if SetBit() works:

    for ( i = 0; i < 320; i++ )
        if ( CheckBit(A, i) ) {
            printf("Bit %d was set !\n", i);
        } else {
            printf("Bit %d was NOT set !\n", i);
        }

    printf("\nClear bit poistions 200 \n");
    ClearBit( A, 200 );

    // Check if ClearBit() works:

    for ( i = 0; i < 320; i++ )
        if ( CheckBit(A, i) )
            printf("Bit %d was set !\n", i);

 * */