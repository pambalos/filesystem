//
// Created by Justice, Bradley on 4/25/20.
//
#include <SystemCalls.h>

struct Dir_Entry * parseInputIntoCommands(struct Dir_Entry *currentDir, char **args, int n) {
    char *command = args[0];
    struct Dir_Entry *workingDir = currentDir;

    if (strcmp(command, "ls") == 0) {
        listDirs(currentDir, &args[0], n-1);
    } else if (strcmp(command, "cd") == 0) {
        workingDir = changeDirectory(workingDir, &args[0], n-1);
    } else if (strcmp(command, "~") == 0) {

    } else if (strcmp(command, "rm") == 0) {

    } else if (strcmp(command, "help") == 0) {

    } else if (strcmp(command, "touch") == 0) {

    }
    return workingDir;
}

void listDirs(struct Dir_Entry *currentDir, char **args, int n) {
    printf("will print directorues\n");
}

struct Dir_Entry * changeDirectory(struct Dir_Entry *currentDir, char **args, int n) {

}