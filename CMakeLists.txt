cmake_minimum_required(VERSION 3.15)
project(filesystem C)

set(CMAKE_C_STANDARD 99)

set(APP_INCLUDE ./include)
set(APP_SRC ./src)

#Include the directories and now your c files will recognize your headers
include_directories(${APP_INCLUDE})
include_directories(${APP_SRC})

set(
        SOURCE_FILES
        ${APP_SRC}/main.c
        ${APP_SRC}/fsLow.c
        ${APP_SRC}/SystemCalls.c
        src/File_System_Info.c
        src/Dir_Entry.c
        src/Free_Blocks.c
        src/BitMap.c
        src/SystemCalls.c
        src/BlockManagementController.c
        )

add_executable(untitled ${SOURCE_FILES})
