cmake_minimum_required(VERSION 3.15)
project(filesystem C)

set(CMAKE_C_STANDARD 99)

set(APP_INCLUDE ./include)
set(APP_SRC ./src)

<<<<<<< HEAD
#Include the directories and now your cpp files will recognize your headers
=======
#Include the directories and now your c files will recognize your headers
>>>>>>> 1f2cc5b4c2afe6f1dfaf2b0806af9814f1b2a320
include_directories(${APP_INCLUDE})
include_directories(${APP_SRC})

set(
        SOURCE_FILES
        ${APP_SRC}/main.c
        ${APP_SRC}/fsLow.c
<<<<<<< HEAD
        ${APP_SRC}/Directory.c
)
=======
        ${APP_SRC}/SystemCalls.c
        src/File_System_Info.c
        src/Dir_Entry.c
        src/Free_Blocks.c
        src/BitMap.c
        src/SystemCalls.c
        src/BlockManagementController.c
        )
>>>>>>> 1f2cc5b4c2afe6f1dfaf2b0806af9814f1b2a320

add_executable(untitled ${SOURCE_FILES})
