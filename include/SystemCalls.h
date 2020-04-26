//
// Created by Justice, Bradley on 4/25/20.
//

#ifndef FILESYSTEM_SYSTEMCALLS_H
#define FILESYSTEM_SYSTEMCALLS_H

#include <string.h>

/**
 * This function will receive the raw arguments and determine what command to call
 * @param n - number of arguments in args
 * @param args - args[0] is the command, &args[0] is the rest
 */
void parseInputIntoCommands(int n, char **args);

#endif //FILESYSTEM_SYSTEMCALLS_H
