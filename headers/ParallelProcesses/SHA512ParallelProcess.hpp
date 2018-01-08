#pragma once

#include <cryptopp/config.h>
#include "ParallelizableProcess.h"

class SHA512ParallelProcess : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C, *D, *E, *F;
    std::vector<byte> retA, retB, retC, retD, retE, retF;
protected:
    void onReady() final;
public:
    SHA512ParallelProcess(std::string key, std::string message);
    ~SHA512ParallelProcess();
};

class SHA512ParallelProcessSimplified : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C;
    std::vector<byte> retA, retB, retC;
protected:
    void onReady() final;
public:
    SHA512ParallelProcessSimplified(std::string key, std::string message);
    ~SHA512ParallelProcessSimplified();
};