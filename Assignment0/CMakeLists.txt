cmake_minimum_required(VERSION 3.8)
project(Assignment1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        mergesort.c
        movie_metadata.csv
        movie_metatest.csv
        sorter.c
        Sorter.h)

add_executable(Assignment1 ${SOURCE_FILES})