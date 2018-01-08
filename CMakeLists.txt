cmake_minimum_required(VERSION 3.5.1)
project(proj)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lcryptopp -pthread")

set(SOURCE_FILES main.cpp headers/Thread.hpp src/Thread.cpp headers/Network/TCPserver.hpp src/Network/TCPserver.cpp headers/ErrorCodes.hpp headers/Utils.hpp src/Utils.cpp headers/Constants.hpp headers/ParallelProcesses/ParallelizableProcess.h src/ParallelProcesses/ParallelizableProcess.cpp headers/SHA512.h src/SHA512.cpp headers/MD5.hpp src/MD5.cpp src/ParallelProcesses/SHA512ParallelProcess.cpp headers/ParallelProcesses/SHA512ParallelProcess.hpp headers/Network/SDISServer.hpp src/Network/SDISServer.cpp)
add_executable(proj ${SOURCE_FILES})