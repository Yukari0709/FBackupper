#ifndef INCLUDE_BACKUPHELPER
#define INCLUDE_BACKUPHELPER

#include "parser.h"
#include "file.h"
#include "Pack.h"
#include "TaskHelper.h"

class BackupHelper : public TaskHelper{
private:

    std::vector<File> all_files;
    
    // 创建的要写入的备份文件路径
    std::string bkfile_path;

    void initBackupFile();
   
    void doPack();
    void doCompress();
    void doEncrypt();

public:
    BackupHelper(const Paras &p);
    // ~BackupHelper();

    void doTask();

};


class Filter : public TaskHelper{
private:
    std::vector<File> all_files;

    void processPath(const std::string& current_path);
    bool checkFilesChangeTime(const struct stat& metadata);
    bool checkFilesModifyTime(const struct stat& metadata);
    bool checkFilesSize(const struct stat& metadata);
    bool checkFilesAddress(const std::string &name);
    bool checkFilesName(const std::string &name);
    bool checkFilesType(const std::string &name);    
public:
    Filter(const Paras &p);
    void doTask();
    std::vector<File> getFiles();
};


class ListHelper : public TaskHelper{
public:
    ListHelper(const Paras &p);
    void doTask();
};

std::filesystem::path changeRELtoABS(std::filesystem::path &path, char *rel_path);

extern std::map<ino_t, std::string> inoToStringMap;

#endif
