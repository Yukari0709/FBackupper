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
    std::string re_path;
    std::vector<int> size;
    std::vector<std::string> ctime;
    std::vector<std::string> mtime;
    std::string passwd;

    std::vector<File> all_files;

    void initBackupFile();
   
    void doPack();
    void doCompress();
    void doEncrypt();
    void processPath(const std::string& current_path);
    bool checkFilesCreationTime(const struct stat& metadata);
    bool checkFilesModifyTime(const struct stat& metadata);
    bool checkFilesSize(const struct stat& metadata);
    bool checkFilesAddress(const struct stat& metadata);
    bool checkFilesName(const struct stat& metadata);
    bool checkFilesType(const struct stat& metadata);

public:
    BackupHelper(const Paras &p);
    // ~BackupHelper();

    void doBackup();
    void doFilter();
    void getctime();
    void getctimetrans();
};

#endif
