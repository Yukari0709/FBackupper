#ifndef TASK_HELPER
#define TASK_HELPER

#include "parser.h"

class TaskHelper{
protected:
    Paras global_paras;
public:
    TaskHelper(const Paras &p) : global_paras(p){};
    virtual void doTask() = 0;
};

#endif
