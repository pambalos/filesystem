//
// Created by Justice, Bradley on 4/25/20.
//
#include <SystemCalls.h>
#include <BlockManagementController.h>
#include <ntsid.h>
#include <time.h>

struct Dir_Entry * parseInputIntoCommands(struct File_System_Info *fs, struct Dir_Entry *currentDir, char **args, int n) {
    char *command = args[0];
    struct Dir_Entry *workingDir = currentDir;

    if (strcmp(command, "ls") == 0) {
        listDirs(currentDir, &args[1], n-1);
    } else if (strcmp(command, "cd") == 0) {
        workingDir = changeDirectory(workingDir, &args[1], n-1);
    } else if (strcmp(command, "~") == 0) {
        //current = root
        workingDir = fs->root;
    } else if (strcmp(command, "rm") == 0) {
        //Grayson

    } else if (strcmp(command, "help") == 0) {
        //adam

    } else if (strcmp(command, "touch") == 0) {
        //bradley
        createFile(fs, currentDir, &args[1], n-1);
    } else if (strcmp(command, "scp") == 0) {
        //adam

    } else if (strcmp(command, "mkdir") == 0) {

    }
    return workingDir;
}

void createFile(struct File_System_Info *fs, struct Dir_Entry *current_directory, char **args, int n) {
    struct Dir_Entry *newFile = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));

    int lbaLocation = findSpace(fs, fs->blockSize);
    newFile->selfAddress = lbaLocation;

    newFile->parentAddress = current_directory->selfAddress;

    //get the name of the file...
    char *buffer = strtok(args[0], ".");
    strcpy(newFile->name, buffer);

    //now get extension
    buffer = strtok(NULL, " ");
    while (strcmp(buffer, "txt") != 0 &&
            strcmp(buffer, "zip") != 0 &&
            strcmp(buffer, "sh") != 0 &&
            strcmp(buffer, "exe") != 0 &&
            strcmp(buffer, "pdf") != 0 &&
            strcmp(buffer, "bat") != 0 &&
            strcmp(buffer, "dir") != 0
            ) {
        buffer = malloc(30);
        printf("ERROR: file type not supported, try txt, zip, sh, exe, pdf, bat, dir\n");
        fgets(buffer, sizeof(buffer), stdin);
    }
    newFile->file_type = 0;
    if (strcmp(buffer, "zip") != 0) {
        newFile->file_type = 1;
    } else if (strcmp(buffer, "sh") != 0) {
        newFile->file_type = 2;
    } else if (strcmp(buffer, "exe") != 0) {
        newFile->file_type = 3;
    } else if (strcmp(buffer, "pdf") != 0) {
        newFile->file_type = 4;
    } else if (strcmp(buffer, "bat") != 0) {
        newFile->file_type = 5;
    } else if (strcmp(buffer, "dir") != 0) {
        newFile->file_type = 6;
    }

    int myInt;
    printf("Enter a permissions level (ie. 644): ");
    int result = scanf("%d", &myInt);
    while (result == EOF) {
        /* ... you're not going to get any input ... */
        printf("try again...\n");
        printf("Enter a permissions level (ie. 644): ");
        result = scanf("%d", &myInt);

    }
    newFile->permissions = myInt;

    time_t t;   // not a primitive datatype
    time(&t);
    newFile->date_created = malloc(sizeof(char) * 30);
    newFile->date_modified = malloc(sizeof(char) * 30);
    strcpy(newFile->date_created, ctime(&t));
    strcpy(newFile->date_modified, newFile->date_created);
    printf("\nfile initialized at (date and time): %s", ctime(&t));

    unsigned long sizeInBlocks = 1; //this size will actually just be num Blocks
    newFile->sizeInBlocks = sizeInBlocks;

    newFile->contentsLocation = 0;

    newFile->numFiles = 0;

    if (current_directory->numFiles == 0) {
        current_directory->fileLBAaddresses = malloc(sizeof(long));
        current_directory->fileLBAaddresses[0] = newFile->selfAddress;
        current_directory->numFiles = 1;
    } else {
        unsigned long *tmp = malloc(sizeof(long)*(current_directory->numFiles+1));
        memcpy(tmp, current_directory->fileLBAaddresses, sizeof(long)*current_directory->numFiles);
        tmp[current_directory->numFiles] = newFile->selfAddress;
        current_directory->numFiles++;
        current_directory->fileLBAaddresses = tmp;
    }
    int c; while ((c = getchar()) != EOF && c != '\n') ;

    char *x = malloc(512);
    LBAwrite(serialize_de(newFile), newFile->sizeInBlocks, lbaLocation);
    LBAwrite(serialize_de(current_directory), current_directory->sizeInBlocks, current_directory->selfAddress);
    LBAread(x, 1, current_directory->selfAddress);
    struct Dir_Entry *test = deserialize_de(x);
    bool aaa = false;
}

void listDirs(struct Dir_Entry *currentDir, char **args, int n) {
    struct Dir_Entry *file;
    bool detailed = false;
    if (n > 0 && strcmp(args[0], "-a") == 0) {
        detailed = true;
    }
    if (detailed) {
        printf(".\n");
        if (currentDir->parentAddress != -1) printf("..\n");
    }
    for (int i = 0; i < currentDir->numFiles; i++) {
        char *buffer = malloc(MINBLOCKSIZE);
        LBAread(buffer, 1, currentDir->fileLBAaddresses[i]);
        file = deserialize_de(buffer);
        if (detailed) {
            printf("%s\t%lu\n", file->name, file->sizeInBlocks);
        } else {
            printf("%s\n", file->name);
        }
    }
    printf("\n");
}

struct Dir_Entry * changeDirectory(struct Dir_Entry *currentDir, char **args, int n) {

}