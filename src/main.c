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
<<<<<<< HEAD
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
=======
#include <SystemCalls.h>
#include <File_System_Info.h>
#include <stdbool.h>
#include <Dir_Entry.h>

/**
 * This function will take the File_System_Info struct in memory and loop, capturing input and dispatching system calls
 * @param fs - the File_System_Info struct to hold the file system in memory
 */
void startFileSystem(struct File_System_Info *fs) {
    struct Dir_Entry *currentDir = fs->root;

    char inputBuffer[1024];
    while (true) {
        printf("$:");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        while (strlen(inputBuffer) == 0) {
            printf("ERROR: No commands entered, please try again\n");
            fgets(inputBuffer, sizeof(inputBuffer), stdin);
            break;
        }

        /**
         * This section was taken from https://stackoverflow.com/questions/11198604/c-split-string-into-an-array-of-strings,
         * Authored by Filip Roseen https://stackoverflow.com/users/1090079/filip-ros%c3%a9en-refp
         * I used this as a way to parse my inputs into an dynamic array of input arguments
         * This takes each token and attempts to add it to the inputs array, and if it encounters a full array, it resizes
         */
        char** inputs = NULL;
        char* input = strtok(inputBuffer, " ");
        int nSpaces = 0;

        while (input) {
            inputs = realloc(inputs, sizeof(char*) * ++nSpaces);
            if (inputs == NULL) {
                printf("Memory allocation for args failed\n");
                exit(-1);
            }
            inputs[nSpaces - 1] = input;
            //This line takes the next input argument, if it encounters EOF, it returns null,
            // which is part of the programming to reach the end of the line, automatically handles EOF
            input = strtok(NULL, " ");
        }
        inputs = realloc(inputs, sizeof(char*) *(nSpaces + 1));
        inputs[nSpaces] = 0;
        inputs[nSpaces - 1] = strtok(inputs[nSpaces - 1], "\n");


        if(nSpaces == 1 && (strcmp(inputs[0], "exit") == 0)) {
            printf("Exiting FileSystem...\n");
            break;
        } else {
            //Pass input to a separate function...
            currentDir = parseInputIntoCommands(currentDir, inputs, nSpaces);
        }
    }
}

int main(int argc, char *argv[]) {

    struct File_System_Info *fs = fsinit(argc, argv);

    startFileSystem(fs);

    closePartitionSystem();
    return 0;
}
>>>>>>> 1f2cc5b4c2afe6f1dfaf2b0806af9814f1b2a320
