#ifndef INCLUDE_PARSER
#define INCLUDE_PARSER

#include <iostream>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include "CLI11.hpp"

#ifdef _WIN32
#include <direct.h>
#elif __APPLE__ || __linux__
#include <unistd.h>
#endif

typedef struct Paras{
    bool help;
    bool backup;
    bool restore;
    bool list;
    std::string input_path;
    std::string output_path;
    bool compress;
    bool encrypt;
    std::string password;
    std::string re_name;
    std::string re_path;
    std::vector<off_t> size;
    std::vector<std::string> ctime;
    std::vector<std::string> mtime;
    std::vector<int> typenum;

    Paras() : backup(false), restore(false), compress(false), encrypt(false), help(false), list(false){}
}Paras;

void Usage();
void doParaParser(int argc, char **argv, Paras &paras);
void initConfig(Paras &paras);


#endif

