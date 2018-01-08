cmake_minimum_required(VERSION 3.8)
project(proj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES main.cpp headers/Thread.hpp src/Thread.cpp headers/TCPserver.hpp src/TCPserver.cpp headers/ErrorCodes.hpp headers/Utils.hpp src/Utils.cpp headers/Constants.hpp headers/ParallelizableProcess.h src/ParallelizableProcess.cpp)
add_executable(proj ${SOURCE_FILES})