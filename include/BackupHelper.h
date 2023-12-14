#ifndef INCLUDE_BACKUPHELPER
#define INCLUDE_BACKUPHELPER

#include "parser.h"
#include "file.h"

class BackupHelper{
private:
    std::string output_path;

    std::string input_path;
    const bool _compress;
    const bool _encrypt;
    std::string re_name;
    std::vector<int> size;
    std::vector<std::string> time;
    std::string passwd;

    std::vector<File> all_files;

    void initBackupFile();
    void doFilter();
    void doPack();
    void doCompress();
    void doEncrypt();

public:
    BackupHelper(const Paras &p);
    // ~BackupHelper();

    void doBackup();
    
};

#endif
