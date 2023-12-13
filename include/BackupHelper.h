#ifndef INCLUDE_BACKUPHELPER
#define INCLUDE_BACKUPHELPER

#include "parser.h"
#include "file.h"

class BackupHelper{
private:
    std::string input_path;
    const bool _compress;
    const bool _encrypt;

public:
    BackupHelper(const Paras &p);
    // ~BackupHelper();

    void doBackup(std::vector<File> file_names, std::string input_path);
};

#endif
