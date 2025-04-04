cmake_minimum_required(VERSION 3.28)
project(DSPD_2_Assignment C)

set(CMAKE_C_STANDARD 11)

add_executable(DSPD_2_Assignment
        main.c
        wlmrt.c
        strbuild.c
        cJSON.c
        system.c
        map.c
        bPLUS_Tree.h
)
