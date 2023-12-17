#ifndef INCLUDE_RESTOREHELPER
#define INCLUDE_RESTOREHELPER

#include "parser.h"
#include "Pack.h"

class RestoreHelper{
private:
    std::string input_name;
    Paras paras;

public:
    RestoreHelper(const Paras &p);
    void doRestore();
};

#endif