#pragma once

#include <set>
#include <vector>
#include <functional>

#include "../Thread.hpp"
#include "../Network/TCPserver.hpp"

class SimpleTask;

class ParallelizableProcess {
    friend class SimpleTask;

    std::set<SimpleTask*> readyExitTasks;

    void setExitTaskReady(SimpleTask*);
    //bool recursiveIsRunning(const SimpleTask *, std::set<const SimpleTask *> &alreadyChecked) const;
    bool recursiveIsRunning(const SimpleTask *) const;
    ThreadCondition joinCondition;
    Mutex joinMutex;


protected:
    static std::vector<std::set<SimpleTask*>> tasksOnCores;
    std::set<SimpleTask*> entryTasks, exitTasks;
    Connection* conn;

    virtual void onReady()=0;
public:
    ParallelizableProcess();
    ParallelizableProcess(Connection* c);
    static unsigned int getBestCore();
    bool isRunning() const;
    void run();
    void join();
};

class SimpleTask {
    friend class ParallelizableProcess;
    std::set<SimpleTask*> nextTasks, prevTasks, readyTasks;
    std::function<void()> task;
    Thread runner;
    ParallelizableProcess* const parent;
    unsigned int runningOnCore=0;

    void setReadyOnAllNext();

    void addPrevTask(SimpleTask*);
    void addPrevTasks(std::set<SimpleTask*>);
    void addNextTask(SimpleTask*);
    void addNextTasks(std::set<SimpleTask*>);
public:
    const std::string name;

    explicit SimpleTask(const std::string & name, ParallelizableProcess *parent, std::function<void()> task,
                        std::set<SimpleTask *> prevTasks = {});
    explicit SimpleTask(ParallelizableProcess *parent, std::function<void()> task,
                        std::set<SimpleTask *> prevTasks = {});
    void setTaskReady(SimpleTask*);
    bool isReady() const;
    bool isRunning() const;
    void run();
};