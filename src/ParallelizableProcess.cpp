#include <iostream>
#include "../headers/ParallelizableProcess.h"

using namespace std;

SimpleTask::SimpleTask(ParallelizableProcess* parent, const std::function<void()>& task, std::set<SimpleTask *> prevTasks) :
        parent(parent),
        task(task),
        runner(task,[this](){ParallelizableProcess::tasksOnCores[runningOnCore].erase(this);this->setReadyOnAllNext();}) {
    addPrevTasks(prevTasks);
}

void SimpleTask::setReadyOnAllNext() {
    if(nextTasks.empty())
        parent->setExitTaskReady(this);
    else
        for(SimpleTask* st: nextTasks) {
            st->setTaskReady(this);
            st->run();
        }
}

void SimpleTask::addPrevTask(SimpleTask * t) {
    if(t == nullptr)
        return;
    if(!isRunning())
        prevTasks.insert(t);
    t->addNextTask(this);
}

void SimpleTask::addPrevTasks(std::set<SimpleTask *> ts) {
    for(SimpleTask* t :ts)
        addPrevTask(t);
}

void SimpleTask::addNextTask(SimpleTask * t) {
    if(t == nullptr)
        return;
    if(!isRunning())
        nextTasks.insert(t);
}

void SimpleTask::addNextTasks(std::set<SimpleTask *> ts) {
    for(SimpleTask* t :ts)
        addNextTask(t);
}

void SimpleTask::setTaskReady(SimpleTask * t) {
    if(t == nullptr)
        return;
    if(prevTasks.find(t)!=prevTasks.end())
        readyTasks.insert(t);
}

bool SimpleTask::isReady() const {
    return readyTasks.size() == prevTasks.size();
}

bool SimpleTask::isRunning() const {
    return runner.isRunning();
}

void SimpleTask::run() {
    if(!isReady() || isRunning())
        return;
    runningOnCore = ParallelizableProcess::getBestCore();
    cout << "Thread started on core " << runningOnCore << endl;
    runner.start();
    runner.setRunningCore(runningOnCore);
    ParallelizableProcess::tasksOnCores[runningOnCore].insert(this);
}

//-------------------------ParallelizableProcess-------------------------//

std::vector<std::set<SimpleTask*>> ParallelizableProcess::tasksOnCores = {{},{},{},{}};

void ParallelizableProcess::setExitTaskReady(SimpleTask * t) {
    if(exitTasks.find(t)!=exitTasks.end())
        readyExitTasks.insert(t);
    if(exitTasks.size()==readyExitTasks.size())
        onReady();
}

//bool ParallelizableProcess::recursiveIsRunning(const SimpleTask *t, set<const SimpleTask *> &alreadyChecked) const {
//    if(t->isRunning())
//        return true;
//    for(const SimpleTask* i : t->nextTasks)
//        if(alreadyChecked.find(i)==alreadyChecked.end()) {
//            if(recursiveIsRunning(i, alreadyChecked))
//                return true;
//            alreadyChecked.insert(i);
//        }
//    return false;
//}

bool ParallelizableProcess::recursiveIsRunning(const SimpleTask *t) const {
    if(t->isRunning())
        return true;
    for(const SimpleTask* i : t->nextTasks)
        if(recursiveIsRunning(i))
            return true;
    return false;
}

unsigned int ParallelizableProcess::getBestCore() {
    if(tasksOnCores.empty())
        throw 1;    //TODO: create exception class
    unsigned int min = 0;
    for(unsigned int i = 1; i<tasksOnCores.size() ; i++)
        if(tasksOnCores[i].size()<tasksOnCores[min].size())
            min=i;
    return min;
}

bool ParallelizableProcess::isRunning() const {
    //set<const SimpleTask*> alreadyChecked;
    for(SimpleTask* t : entryTasks)
        //if(recursiveIsRunning(t, alreadyChecked))
        if(recursiveIsRunning(t))
            return true;
    return false;
}

void ParallelizableProcess::run() {
    if(isRunning())
        return;
    for(SimpleTask* t : entryTasks)
        t->run();
}