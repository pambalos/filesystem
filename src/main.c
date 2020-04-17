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

int main(int argc, char *argv[])
{

    struct File_System_Info *fs = fsinit(argc, argv);

    closePartitionSystem();
    return 0;
}