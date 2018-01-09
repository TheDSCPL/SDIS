#pragma once

#include "ParallelizableProcess.h"

class PrimesParallelProcess : public ParallelizableProcess {
    unsigned long long int maxN;
    std::set<SimpleTask*> primeCalculators;
    std::vector<bool> flags;
    unsigned long long int cursor;
    unsigned long long int maxCursor;
    Mutex cursorMutex;
protected:
    void onReady() final;
public:
    PrimesParallelProcess(Connection* ,unsigned long long int);
    ~PrimesParallelProcess();
};