#ifndef INCLUDE_PARSER
#define INCLUDE_PARSER

#include <iostream>
#include "CLI11.hpp"

typedef struct Paras{
    bool backup;
    bool restore;
    std::string input_path;
    std::string output_path;
    bool compress;
    bool encrypt;
    std::string password;
    std::string re_name;

    Paras() : backup(false), restore(false), compress(false), encrypt(false){}
}Paras;

void Usage();
void doParaParser(int argc, char **argv);

// declare
extern Paras paras;

#endif

