#pragma once

#include <cryptopp/config.h>
#include "ParallelizableProcess.h"

class HMACSHA512SeriesProcess : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C, *D, *E, *F;
    std::vector<byte> retA, retB, retC, retD, retE, retF;
protected:
    void onReady() final;
public:
    HMACSHA512SeriesProcess(Connection*, std::string key, std::string message);
    ~HMACSHA512SeriesProcess();
};

class HMACSHA512ParallelProcess : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C, *D, *E, *F;
    std::vector<byte> retA, retB, retC, retD, retE, retF;
protected:
    void onReady() final;
public:
    HMACSHA512ParallelProcess(Connection*, std::string key, std::string message);
    ~HMACSHA512ParallelProcess();
};

class HMACSHA512ParallelProcessSimplified : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C;
    std::vector<byte> retA, retB, retC;
protected:
    void onReady() final;
public:
    HMACSHA512ParallelProcessSimplified(Connection*, std::string key, std::string message);
    ~HMACSHA512ParallelProcessSimplified();
};

class HMACMD5SeriesProcess : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C, *D, *E, *F;
    std::vector<byte> retA, retB, retC, retD, retE, retF;
protected:
    void onReady() final;
public:
    HMACMD5SeriesProcess(Connection*, std::string key, std::string message);
    ~HMACMD5SeriesProcess();
};

class HMACMD5ParallelProcess : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C, *D, *E, *F;
    std::vector<byte> retA, retB, retC, retD, retE, retF;
protected:
    void onReady() final;
public:
    HMACMD5ParallelProcess(Connection*, std::string key, std::string message);
    ~HMACMD5ParallelProcess();
};

class HMACMD5ParallelProcessSimplified : public ParallelizableProcess {
    std::string key, message;
    SimpleTask *A, *B, *C;
    std::vector<byte> retA, retB, retC;
protected:
    void onReady() final;
public:
    HMACMD5ParallelProcessSimplified(Connection*, std::string key, std::string message);
    ~HMACMD5ParallelProcessSimplified();
};