#ifndef INCLUDE_BACKUPHELPER
#define INCLUDE_BACKUPHELPER

#include "parser.h"
#include "file.h"
#include "Pack.h"

class BackupHelper{
private:
    std::string output_path;

    std::string input_path;
    const bool _compress;
    const bool _encrypt;
    std::string re_name;
    std::string re_path;
    std::vector<int> size;
    std::vector<std::string> ctime;
    std::vector<std::string> mtime;
    std::string passwd;

    std::vector<File> all_files;
    
    // 创建的要写入的备份文件路径
    std::string bkfile_path;

    void initBackupFile();
    void doFilter();
    void doPack();
    void doCompress();
    void doEncrypt();
    void processPath(const std::string& current_path);

public:
    BackupHelper(const Paras &p);
    // ~BackupHelper();

    void doBackup();
};

extern std::map<ino_t, std::string> inoToStringMap;

#endif
