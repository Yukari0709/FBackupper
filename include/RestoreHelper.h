#ifndef INCLUDE_RESTOREHELPER
#define INCLUDE_RESTOREHELPER

#include "parser.h"
#include "Pack.h"
#include "TaskHelper.h"

class RestoreHelper : public TaskHelper{
private:
    std::string input_name;

public:
    RestoreHelper(const Paras &p);
    void doTask();
};

#endif