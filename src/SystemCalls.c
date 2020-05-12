//
// Created by Justice, Bradley on 4/25/20.
//
#include <SystemCalls.h>
#include <BlockManagementController.h>
#include <ntsid.h>
#include <time.h>

void testSerialization(struct File_System_Info *fs, struct Dir_Entry *currentDir, char **args);

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
        workingDir = removeDir(fs, currentDir, &args[1], n-1);
    } else if (strcmp(command, "help") == 0) {
        //adam
        helpFunc(&args[1], n-1);
    } else if (strcmp(command, "touch") == 0) {
        //bradley
        createFile(fs, currentDir, &args[1], n-1);
    } else if (strcmp(command, "scp") == 0) {
        copyToFromLinux(fs, currentDir, &args[1], n-1);
    } else if (strcmp(command, "mkdir") == 0) {
        createDirectory(fs, currentDir, &args[1], n-1);
    } else if (strcmp(command, "mdset") == 0) {
        createExplicitFile(fs, currentDir, &args[1], n-1);
    } else if (strcmp(command, "test") == 0) {
        testSerialization(fs, currentDir, &args[1]);
    }
    return workingDir;


void copyToFromLinux(struct File_System_Info *fs, struct Dir_Entry *currentDir, char **args, int n) {
    if (strcmp(args[0], "-t") == 0) { //to linux

        struct Dir_Entry *file;
        for (int i = 0; i < currentDir->numFiles; i++) {
            char *buffer = malloc(MINBLOCKSIZE);
            LBAread(buffer, 1, currentDir->fileLBAaddresses[i]);
            file = deserialize_de(buffer);
            if (strcmp(file->name, args[1]) == 0) { //found file to copy to linux
                FILE *fp;
                fp = fopen (file->name, "w");
                fputs("FILE CONTENT", fp);
                fclose (fp); //close after writing data...
            }
        }
        //linux file stuff
    } else if (strcmp(args[0], "-f") == 0) { //from linux
        struct Dir_Entry *file = malloc(MINBLOCKSIZE);
        FILE *fp;
        fp = fopen (args[0], "r");
        //this is where we would copy stuff over

        strcpy(file->name, args[1]);
        int freeSpace = findSpace(fs, MINBLOCKSIZE);
        file->selfAddress = freeSpace;
        file->parentAddress = currentDir->selfAddress;
        file->file_type = 1;
        time_t t;   // not a primitive datatype
        time(&t);
        file->date_created = malloc(sizeof(char) * 30);
        file->date_modified = malloc(sizeof(char) * 30);
        strcpy(file->date_created, ctime(&t));
        strcpy(file->date_modified, file->date_created);
        printf("\nNew file copied from Linux at (date and time): %s", ctime(&t));
        file->sizeInBlocks = 1;
        file->numFiles = 0;
        file->fileLBAaddresses = NULL;

        if (currentDir->numFiles == 0) {
            currentDir->fileLBAaddresses = malloc(sizeof(long));
            currentDir->fileLBAaddresses[0] = file->selfAddress;
            currentDir->numFiles = 1;
        } else {
            unsigned long *tmp = malloc(sizeof(long)*(currentDir->numFiles+1));
            memcpy(tmp, currentDir->fileLBAaddresses, sizeof(long)*currentDir->numFiles);
            *(tmp+(currentDir->numFiles)) = file->selfAddress;
            currentDir->numFiles++;
            currentDir->fileLBAaddresses = tmp;
        }
        //int c; while ((c = getchar()) != EOF && c != '\n') ;

        LBAwrite(serialize_de(file), file->sizeInBlocks, file->selfAddress);
        LBAwrite(serialize_de(currentDir), currentDir->sizeInBlocks, currentDir->selfAddress);
        SetBit(fs->Free_Blocks->fbs, file->selfAddress);

    } else {
        printf("Invalid input... try again\n");
    }
}

void testSerialization(struct File_System_Info *fs, struct Dir_Entry *currentDir, char **args) {
    char *buffer = malloc(MINBLOCKSIZE);
    LBAwrite(serialize_de(currentDir), 1, 6);
    LBAread(buffer, 1, 6);
    struct Dir_Entry *tdir = deserialize_de(buffer);

    bool t;
}

struct Dir_Entry *removeDir(struct File_System_Info *fs, struct Dir_Entry *currentDir, char **args, int n) {
    char *fileName = args[0];
    struct Dir_Entry *file;
    for (int i = 0; i < currentDir->numFiles; i++) {
        char *buffer = malloc(MINBLOCKSIZE);
        LBAread(buffer, 1, currentDir->fileLBAaddresses[i]);
        file = deserialize_de(buffer);
        if (strcmp(file->name, fileName) == 0) {
            unsigned long toRemove = file->selfAddress;
            //remove the file address from address list
            unsigned long *addresses = malloc(sizeof(unsigned long)*(currentDir->numFiles-1));
            memset(addresses, 0, currentDir->numFiles-1);
            int c = 0;
            for (int j = 0; j < currentDir->numFiles; j++) {
                if (*(currentDir->fileLBAaddresses+j) != toRemove) {
                    memcpy((addresses+c), (currentDir->fileLBAaddresses+j), sizeof(long));
                    c++;
                }
            }
            currentDir->numFiles--;
            if (currentDir->numFiles > 1) {
                currentDir->fileLBAaddresses = NULL;
            } else {
                currentDir->fileLBAaddresses = addresses;
            }
            //now free the bits in the map and blocks
            char * b = malloc(512);
            memset(b, 0, 512);
            //LBAwrite(b, 1, file->contentsLocation);
            //LBAwrite(b, 1, file->selfAddress);
            ClearBit(fs->Free_Blocks->fbs, file->selfAddress);
            ClearBit(fs->Free_Blocks2->fbs, file->selfAddress);

            LBAwrite(serialize_de(currentDir), currentDir->sizeInBlocks, currentDir->selfAddress);
        }
    }

    return currentDir;
}

/**
 * Copy & Pasted from Steven's branch to avoid merge issues
 * @param fs
 * @param current_directory
 * @param args
 * @param n
 */
void createDirectory(struct File_System_Info *fs, struct Dir_Entry *current_directory, char **args, int n) {
    struct Dir_Entry *newDirectory = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));

    int lbaLocation = findSpace(fs, fs->blockSize);
    newDirectory->selfAddress = lbaLocation;

    newDirectory->parentAddress = current_directory->selfAddress;

    //get the name of the file...
    char *buffer = strtok(args[0], "\n");
    strcpy(newDirectory->name, buffer);

    newDirectory->file_type = 6;

    int myInt;
    printf("Enter a permissions level (ie. 644): ");
    int result = scanf("%d", &myInt);
    while (result == EOF) {
        /* ... you're not going to get any input ... */
        printf("try again...\n");
        printf("Enter a permissions level (ie. 644): ");
        result = scanf("%d", &myInt);
    }
    newDirectory->permissions = result;

    time_t t;   // not a primitive datatype
    time(&t);
    newDirectory->date_created = malloc(sizeof(char) * 30);
    newDirectory->date_modified = malloc(sizeof(char) * 30);
    strcpy(newDirectory->date_created, ctime(&t));
    strcpy(newDirectory->date_modified, newDirectory->date_created);
    printf("\ndirectory initialized at (date and time): %s", ctime(&t));

    unsigned long sizeInBlocks = 1; //this size will actually just be num Blocks
    newDirectory->sizeInBlocks = sizeInBlocks;

    newDirectory->contentsLocation = 0;

    newDirectory->numFiles = 0;

    if (current_directory->numFiles == 0) {
        current_directory->fileLBAaddresses = malloc(sizeof(long));
        current_directory->fileLBAaddresses[0] = newDirectory->selfAddress;
        current_directory->numFiles = 1;
    } else {
        unsigned long *tmp = malloc(sizeof(long)*(current_directory->numFiles+1));
        /*
        for (int i = 0; i < current_directory->numFiles; i++) {
            unsigned long myL =0l;
            memcpy(&myL, (current_directory->fileLBAaddresses+i), sizeof(long));
            memcpy((tmp+i), (current_directory->fileLBAaddresses+i), sizeof(long));
        } */

        memcpy(tmp, current_directory->fileLBAaddresses, sizeof(long)*current_directory->numFiles);
        *(tmp+(current_directory->numFiles)) = newDirectory->selfAddress;
        current_directory->numFiles++;
        current_directory->fileLBAaddresses = tmp;
    }
    int c; while ((c = getchar()) != EOF && c != '\n') ;

    SetBit(fs->Free_Blocks->fbs, newDirectory->selfAddress);
    SetBit(fs->Free_Blocks2->fbs, newDirectory->selfAddress);
    LBAwrite(serialize_de(newDirectory), newDirectory->sizeInBlocks, lbaLocation);
    LBAwrite(serialize_de(current_directory), current_directory->sizeInBlocks, current_directory->selfAddress);
}

void createFile(struct File_System_Info *fs, struct Dir_Entry *current_directory, char **args, int n) {
    struct Dir_Entry *newFile = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));

    int lbaLocation = findSpace(fs, fs->blockSize);
    newFile->selfAddress = lbaLocation;

    newFile->parentAddress = current_directory->selfAddress;

    //get the name of the file...
    char *buffer = strtok(args[0], ".");
    strcpy(newFile->name, buffer);

    newFile->file_type = 0;
    //now get extension
    buffer = strtok(NULL, " ");
    if (buffer != NULL) {
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
    } else {
        newFile->file_type = 0;
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
        /*
        for (int i = 0; i < current_directory->numFiles; i++) {
            unsigned long myL =0l;
            memcpy(&myL, (current_directory->fileLBAaddresses+i), sizeof(long));
            memcpy((tmp+i), (current_directory->fileLBAaddresses+i), sizeof(long));
        } */

        memcpy(tmp, current_directory->fileLBAaddresses, sizeof(long)*current_directory->numFiles);
        *(tmp+(current_directory->numFiles)) = newFile->selfAddress;
        current_directory->numFiles++;
        current_directory->fileLBAaddresses = tmp;
    }
    int c; while ((c = getchar()) != EOF && c != '\n') ;

    LBAwrite(serialize_de(newFile), newFile->sizeInBlocks, lbaLocation);
    LBAwrite(serialize_de(current_directory), current_directory->sizeInBlocks, current_directory->selfAddress);
    SetBit(fs->Free_Blocks->fbs, lbaLocation);
}

void createExplicitFile(struct File_System_Info *fs, struct Dir_Entry *current_directory, char **args, int n) {
    struct Dir_Entry *newExplicitFile = (struct Dir_Entry*)malloc(sizeof(struct Dir_Entry));

    int lbaLocation = findSpace(fs, fs->blockSize);
    newExplicitFile->selfAddress = lbaLocation;

    newExplicitFile->parentAddress = current_directory->selfAddress;

    //get the name of the file...
    char *buffer = strtok(args[0], ".");
    strcpy(newExplicitFile->name, buffer);

    //now get extension
    buffer = strtok(NULL, " ");
    if (buffer != NULL) {
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
        newExplicitFile->file_type = 0;
        if (strcmp(buffer, "zip") != 0) {
            newExplicitFile->file_type = 1;
        } else if (strcmp(buffer, "sh") != 0) {
            newExplicitFile->file_type = 2;
        } else if (strcmp(buffer, "exe") != 0) {
            newExplicitFile->file_type = 3;
        } else if (strcmp(buffer, "pdf") != 0) {
            newExplicitFile->file_type = 4;
        } else if (strcmp(buffer, "bat") != 0) {
            newExplicitFile->file_type = 5;
        } else if (strcmp(buffer, "dir") != 0) {
            newExplicitFile->file_type = 6;
        }
    } else {
        newExplicitFile->file_type = 0;
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
    newExplicitFile->permissions = myInt;

    time_t t;   // not a primitive datatype
    time(&t);
    newExplicitFile->date_created = malloc(sizeof(char) * 30);
    newExplicitFile->date_modified = malloc(sizeof(char) * 30);
    strcpy(newExplicitFile->date_created, ctime(&t));
    strcpy(newExplicitFile->date_modified, newExplicitFile->date_created);
    printf("\nfile initialized at (date and time): %s", ctime(&t));

    unsigned long sizeInBlocks;
    printf("Enter number of blocks for file: ");
    int result2 = scanf("%d", &sizeInBlocks);
    while (result2 == EOF) {
        printf("try again...\n");
        printf("Enter number of blocks for file: ");
        result2 = scanf("%d", &sizeInBlocks);
    }
    newExplicitFile->sizeInBlocks = sizeInBlocks;

    unsigned long contentsLocation;
    printf("Enter content starting location (default is 0): ");
    int result3 = scanf("%d", &contentsLocation);
    while (result3 == EOF) {
        printf("try again...\n");
        printf("Enter content starting location (default is 0): ");
        result3 = scanf("%d", &contentsLocation);
    }
    newExplicitFile->contentsLocation = contentsLocation;

    int numFiles;
    printf("Enter number of files (default is 0): ");
    int result4 = scanf("%d", &numFiles);
    while (result4 == EOF) {
        printf("try again...\n");
        printf("Enter number of files (default is 0): ");
        result4 = scanf("%d", &numFiles);
    }
    newExplicitFile->numFiles = numFiles;

    if (current_directory->numFiles == 0) {
        current_directory->fileLBAaddresses = malloc(sizeof(long));
        current_directory->fileLBAaddresses[0] = newExplicitFile->selfAddress;
        current_directory->numFiles = 1;
    } else {
        unsigned long *tmp = malloc(sizeof(long)*(current_directory->numFiles+1));
        /*
        for (int i = 0; i < current_directory->numFiles; i++) {
            unsigned long myL =0l;
            memcpy(&myL, (current_directory->fileLBAaddresses+i), sizeof(long));
            memcpy((tmp+i), (current_directory->fileLBAaddresses+i), sizeof(long));
        } */

        memcpy(tmp, current_directory->fileLBAaddresses, sizeof(long)*current_directory->numFiles);
        *(tmp+(current_directory->numFiles)) = newExplicitFile->selfAddress;
        current_directory->numFiles++;
        current_directory->fileLBAaddresses = tmp;
    }
    int c; while ((c = getchar()) != EOF && c != '\n') ;

    LBAwrite(serialize_de(newExplicitFile), newExplicitFile->sizeInBlocks, lbaLocation);
    LBAwrite(serialize_de(current_directory), current_directory->sizeInBlocks, current_directory->selfAddress);
    SetBit(fs->Free_Blocks->fbs, lbaLocation);
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
            if (file->file_type == 6) {
                printf("%s/\n", file->name);
            } else {
                printf("%s\n", file->name);
            }

        }
    }
    printf("\n");
}

struct Dir_Entry * changeDirectory(struct Dir_Entry *currentDir, char **args, int n) {
    LBAwrite(serialize_de(currentDir), 1, currentDir->selfAddress);
    char *dirName = args[0];
    if (strcmp(dirName, "..") == 0) {
        if (currentDir->parentAddress != -1) {
            char *buffer = malloc(MINBLOCKSIZE);
            LBAread(buffer, 1, currentDir->parentAddress);
            struct Dir_Entry *entry = deserialize_de(buffer);
            //free(buffer);
            return entry;
        }
    }
    for (int i = 0; i < currentDir->numFiles; i++) {
        char *buffer = malloc(MINBLOCKSIZE);
        LBAread(buffer, 1, currentDir->fileLBAaddresses[i]);
        struct Dir_Entry *entry = deserialize_de(buffer);
        if (strcmp(dirName, entry->name) == 0) {
            if (entry->file_type == 6) {
                //free(buffer);
                return entry;
            }
        }
    }
    return currentDir;
}

void helpFunc(char** args, int n) {
    if (n > 0) {
        if (strcmp(args[0], 'ls') == 0){
        printf("ls: ls [-a]\n");
        printf("    Lists the current directory.\n");
        printf("\n");
        printf("    Displays the current directory and resulting path from the ROOT directory\n");
        printf("    if specified.");
        printf("\n");
        printf("    Options:\n");
        printf("     -a:  Displays the current directory detailed.\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success always.\n")
    } else if (strcmp(args[0], 'cd') == 0){
        printf("cd: help [pattern.extension]\n");
        printf("    Changes the current direcotry to the directory specified by pattern.extension.\n");
        printf("\n");
        printf("    Changes the current working directory to the specified directory represented by\n");
        printf("    pattern.extension. Supports various extensions.")
        printf("\n");
        printf("    Extensions:\n");
        printf("     txt\n");
        printf("     zip\n");
        printf("     sh\n");
        printf("     exe\n");
        printf("     pdf\n");
        printf("     bat\n");
        printf("     dir\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success unless PATTERN is not found or an invalid extension option is given.\n")
    } else if (strcmp(args[0], '~') == 0){
        printf("~: ~/[pattern...]\n");
        printf("    Acts as a abbreviation for the user's home directory.\n");
        printf("\n");
        printf("    Acts as an abbreviation or shortcut representation of.\n");
        printf("    the users home or root directory. The following [pattern]\n");
        printf("    is then the remaining part of the non-abbreviated address.\n");
        printf("\n");
        printf("    Arguments:\n");
        printf("     PATTERN:  Pattern specifying an address\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success unless PATTERN is not found.\n")
    } else if (strcmp(args[0], 'rm') == 0){
        printf("rm: rm [pattern...]\n");
        printf("    Removes file at destination matching PATTERN.\n");
        printf("\n");
        printf("    Removes the file matching PATTERN at the specified location.\n");
        printf("    This can be a file, directory, or symbolic link, and the\n");
        printf("    command checks to see if its a valid file for deletion.\n");
        printf("\n");
        printf("    Arguments:\n");
        printf("     PATTERN:  Pattern specifying location of file for deletion.\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success unless PATTERN is not valid or unwriteable, operation\n")
        printf("    is aborted in the later circumstance.\n");
    } else if (strcmp(args[0], 'help') == 0){
        printf("help: help [pattern...]\n");
        printf("    Display information about builtin commands.\n");
        printf("\n");
        printf("    Displays brief summaries of builtin commands. if PATTERN is\n");
        printf("    specified, gives detailed hlep on all commands matching PATTERN,\n");
        printf("    otherwise the list of help topics is printed.\n");
        printf("\n");
        printf("    Arguments:\n");
        printf("     PATTERN:  Pattern specifying a help topic\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success unless PATTERN is not found or an invalid option is given.\n");
    } else if (strcmp(args[0], 'touch') == 0){
        printf("touch: touch [pattern...]\n");
        printf("    Creates an empty file in the current directory with filename PATTERN.\n");
        printf("\n");
        printf("    Creates an empty file with no contents with the filename of PATTERN,\n");
        printf("    either at the location specified in PATTERN, or in the current directory\n");
        printf("    if no location is specified.\n");
        printf("\n");
        printf("    Arguments:\n");
        printf("     PATTERN:  Pattern specifying a filename and/or location for file creation.\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success unless PATTERN is not found or an invalid option is given.\n")
    } else if (strcmp(args[0], 'scp') == 0){
        printf("scp: scp [-rf] [pattern1...] [pattern2...]\n");
        printf("    Moves files from one file system to the other, depending on flags.\n");
        printf("\n");
        printf("    Moves files located in one file system at destination specified\n");
        printf("    by PATTERN1 to the location in the other file system at the\n");
        printf("    destination specified by PATTERN2. Direction is based on argument flags.\n");
        printf("\n");
        printf("    Options:\n");
        printf("     -f: moves it forward from our file system to host file system.\n");
        printf("     -r: moves it reverse from host file system to our file system.\n");
        printf("\n");
        printf("    Arguments:\n");
        printf("     PATTERN1:  First file to be moved.\n");
        printf("     PATTERN2:  Location first file will be moved to.\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success unless PATTERN1 is not found or PATTERN1 or 2 are invalid.\n")
    } else if (strcmp(args[0], 'mkdir') == 0){
        printf("mkdir: mkdir [pattern...]\n");
        printf("    Creates a directory at the location specified by PATTERN.\n");
        printf("\n");
        printf("    Creates the directory at PATTERN named PATTERN if said directory\n");
        printf("    does not already exist.,\n");
        printf("\n");
        printf("    Arguments:\n");
        printf("     PATTERN:  Name and location of directory to be created.\n");
        printf("\n");
        printf("    Exit Status:\n");
        printf("    Return success unless a directory already exists at that location.\n")
    } else {
        printf("The 'help' command must be followed by a command to request help about.\n");
        printf("List of possible commands: 'ls', 'cd', '~', 'rm', 'help', 'touch', 'scp', 'mkdir'\n");
    }
   
}

