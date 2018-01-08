#include <iostream>
#include "../../headers/ParallelProcesses/SHA512ParallelProcess.hpp"
#include "../../headers/SHA512.h"

SHA512ParallelProcess::SHA512ParallelProcess(std::string key, std::string message) : key(key), message(message) {
    A=new SimpleTask(this,[this,key,message](){
        retA=outputPadSHA512(key);
    });
    B=new SimpleTask(this,[this,key,message](){
        retB=inputPadSHA512(key);
    });
    C=new SimpleTask(this,[this,key,message](){
        retC=concat(retB,message);
    },{B});
    D=new SimpleTask(this,[this,key,message](){
        retD=SHA512(retC);
    },{C});
    E=new SimpleTask(this,[this,key,message](){
        retE=concat(retA,retD);
    },{A,D});
    F=new SimpleTask(this,[this,key,message](){
        retF=SHA512(retE);
    },{E});

    entryTasks={A,B};
    exitTasks={F};
}

SHA512ParallelProcess::~SHA512ParallelProcess() {
    delete A, B, C, D, E, F;
}

void SHA512ParallelProcess::onReady() {
    std::cout << bytes2HexString(retF) << std::endl;
}

SHA512ParallelProcessSimplified::SHA512ParallelProcessSimplified(std::string key, std::string message) : key(key), message(message) {
    A=new SimpleTask(this,[this,key,message](){
        retA=outputPadSHA512(key);
    });
    B=new SimpleTask(this,[this,key,message](){
        retB=SHA512(concat(inputPadSHA512(key),message));
    });
    C=new SimpleTask(this,[this,key,message](){
        retC=SHA512(concat(retA,retB));
    },{A,B});

    entryTasks={A,B};
    exitTasks={C};
}

SHA512ParallelProcessSimplified::~SHA512ParallelProcessSimplified() {
    delete A, B, C;
}

void SHA512ParallelProcessSimplified::onReady() {
    std::cout << bytes2HexString(retC) << std::endl;
}