//
// Created by Justice, Bradley on 4/25/20.
//
#include <SystemCalls.h>

void createFile(struct Dir_Entry *pEntry, char **pString, int i);

struct Dir_Entry * parseInputIntoCommands(struct Dir_Entry *currentDir, char **args, int n) {
    char *command = args[0];
    struct Dir_Entry *workingDir = currentDir;

    if (strcmp(command, "ls") == 0) {
        listDirs(currentDir, &args[1], n-1);
    } else if (strcmp(command, "cd") == 0) {
        workingDir = changeDirectory(workingDir, &args[1], n-1);
    } else if (strcmp(command, "~") == 0) {
        //current = root

    } else if (strcmp(command, "rm") == 0) {
        //Grayson

    } else if (strcmp(command, "help") == 0) {
        //adam

    } else if (strcmp(command, "touch") == 0) {
        //bradley
        createFile(currentDir, &args[1], n-1);
    } else if (strcmp(command, "scp") == 0) {
        //adam

    } else if (strcmp(command, "mkdir") == 0) {

    }
    return workingDir;
}

void createFile(struct Dir_Entry *current_directory, char **args, int n) {
    struct Dir_Entry *newFile = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));
    printf("Enter the files name: ");
    char *buffer = malloc(30);
    fgets(buffer, sizeof(buffer), stdin);
    strcpy(newFile->name, "home\n");
    newFile->parentAddress = current_directory->selfAddress;

    
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