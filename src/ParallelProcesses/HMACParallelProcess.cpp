#include <iostream>
#include "../../headers/ParallelProcesses/HMACParallelProcess.hpp"
#include "../../headers/SHA512.h"
#include "../../headers/MD5.hpp"

HMACSHA512ParallelProcess::HMACSHA512ParallelProcess(Connection* c, std::string key, std::string message) : ParallelizableProcess(c), key(key), message(message) {
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

HMACSHA512ParallelProcess::~HMACSHA512ParallelProcess() {
    delete A, B, C, D, E, F;
}

void HMACSHA512ParallelProcess::onReady() {
    if(conn)
        (*conn) << bytes2HexString(retF) << "\n";
    else
        std::cout << bytes2HexString(retF) << std::endl;
}

HMACSHA512ParallelProcessSimplified::HMACSHA512ParallelProcessSimplified(Connection* c, std::string key, std::string message) : ParallelizableProcess(c), key(key), message(message) {
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

HMACSHA512ParallelProcessSimplified::~HMACSHA512ParallelProcessSimplified() {
    delete A, B, C;
}

void HMACSHA512ParallelProcessSimplified::onReady() {
    if(conn)
        (*conn) << bytes2HexString(retC) << "\n";
    else
        std::cout << bytes2HexString(retC) << std::endl;
}





HMACMD5ParallelProcess::HMACMD5ParallelProcess(Connection* c, std::string key, std::string message) : ParallelizableProcess(c), key(key), message(message) {
    A=new SimpleTask(this,[this,key,message](){
        retA=outputPadMD5(key);
    });
    B=new SimpleTask(this,[this,key,message](){
        retB=inputPadMD5(key);
    });
    C=new SimpleTask(this,[this,key,message](){
        retC=concat(retB,message);
    },{B});
    D=new SimpleTask(this,[this,key,message](){
        retD=MD5(retC);
    },{C});
    E=new SimpleTask(this,[this,key,message](){
        retE=concat(retA,retD);
    },{A,D});
    F=new SimpleTask(this,[this,key,message](){
        retF=MD5(retE);
    },{E});

    entryTasks={A,B};
    exitTasks={F};
}

HMACMD5ParallelProcess::~HMACMD5ParallelProcess() {
    delete A, B, C, D, E, F;
}

void HMACMD5ParallelProcess::onReady() {
    if(conn)
        (*conn) << bytes2HexString(retF) << "\n";
    else
        std::cout << bytes2HexString(retF) << std::endl;
}

HMACMD5ParallelProcessSimplified::HMACMD5ParallelProcessSimplified(Connection* c, std::string key, std::string message) : ParallelizableProcess(c), key(key), message(message) {
    A=new SimpleTask(this,[this,key,message](){
        retA=outputPadMD5(key);
    });
    B=new SimpleTask(this,[this,key,message](){
        retB=MD5(concat(inputPadMD5(key),message));
    });
    C=new SimpleTask(this,[this,key,message](){
        retC=MD5(concat(retA,retB));
    },{A,B});

    entryTasks={A,B};
    exitTasks={C};
}

HMACMD5ParallelProcessSimplified::~HMACMD5ParallelProcessSimplified() {
    delete A, B, C;
}

void HMACMD5ParallelProcessSimplified::onReady() {
    if(conn)
        (*conn) << bytes2HexString(retC) << "\n";
    else
        std::cout << bytes2HexString(retC) << std::endl;
}