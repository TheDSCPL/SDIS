#include <cmath>
#include <iostream>
#include "../../headers/ParallelProcesses/PrimesParallelProcess.hpp"

using namespace std;

PrimesParallelProcess::PrimesParallelProcess(Connection* c, unsigned long long int maxN) : ParallelizableProcess(c), maxN(maxN) {
    flags.push_back(false);
    for(int i = 1; i<maxN ; i++)
        flags.push_back(true);

    cursor=0;
    //maxCursor = (unsigned long long int)ceil(sqrt(maxN));
    maxCursor=maxN-1;

    long nCores = Thread::getNCores();
    //cerr << "maxCursor: " << maxCursor << endl;
    for(long i = 0 ; i<nCores ; i++)
        primeCalculators.insert(new SimpleTask("PrimeCalculator",this,[this](){
            unsigned long long int thisN;
            while(true) {
                do {
                    cursorMutex.lock();
                    thisN = ++cursor;
                    cursorMutex.unlock();
                } while(!flags[thisN] && thisN < maxCursor);
                if(thisN >= maxCursor)
                    break;
                for (unsigned long long int j = 2; j*(thisN+1)-1 < this->maxN ; j++)
                    flags[j*(thisN+1)-1]=false;
                    //cout << j*(thisN+1)-1 << endl;
            }
        }));
    entryTasks=primeCalculators;
    //cout << "entryTasks.size() = " << entryTasks.size() << endl;
    exitTasks=primeCalculators;
}

PrimesParallelProcess::~PrimesParallelProcess() {
    for(SimpleTask* t : primeCalculators)
        delete t;
}

void PrimesParallelProcess::onReady() {
    for(int i = 0 ; i<maxN ; i++) {
        if(flags[i]) {
            if (conn)
                (*conn) << i + 1 << " ";
            else
                cout << i + 1 << ' ';
        }
    }
    if (conn)
        (*conn) << "\n";
    else
        cout << endl;
}
