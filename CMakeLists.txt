cmake_minimum_required(VERSION 3.15)
project(filesystem C)

set(CMAKE_C_STANDARD 99)

add_executable(filesystem main.c Directory.c fsLowDriver.c fsLow.c)