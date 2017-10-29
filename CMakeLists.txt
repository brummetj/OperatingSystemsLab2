cmake_minimum_required(VERSION 3.8)
project(OperatingSystemsLab2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        container.h
        main.cpp
        process.h process.cpp)

add_executable(OperatingSystemsLab2 ${SOURCE_FILES})