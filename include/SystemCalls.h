//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_SYSTEMCALLS_H
#define FILESYSTEM_SYSTEMCALLS_H

#include <string.h>
#include <File_System_Info.h>

/**
 * This function will receive the raw arguments and determine what command to call
 * @param n - number of arguments in args
 * @param args - args[0] is the command, &args[0] is the rest
 */
struct Dir_Entry * parseInputIntoCommands(struct Dir_Entry *currentDir, char **args, int n);

void listDirs(struct Dir_Entry *currentDir, char **args, int n);

struct Dir_Entry * changeDirectory(struct Dir_Entry *currentDir, char **args, int n);

void printHomeAdvice();

void rm();

void help();

#endif //FILESYSTEM_SYSTEMCALLS_H
