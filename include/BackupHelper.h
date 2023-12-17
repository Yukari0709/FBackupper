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
    std::vector<off_t> size;
    std::vector<std::string> ctime;
    std::vector<std::string> mtime;
    std::string passwd;
    std::vector<int> typenum;
    std::vector<File> all_files;
    
    // 创建的要写入的备份文件路径
    std::string bkfile_path;

    void initBackupFile();
   
    void doPack();
    void doCompress();
    void doEncrypt();
    void processPath(const std::string& current_path);
    bool checkFilesChangeTime(const struct stat& metadata);
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
    void gettype();
    void getctimetrans();
    void getmtimetrans();
};

std::filesystem::path changeRELtoABS(std::filesystem::path &path, char *rel_path);

extern std::map<ino_t, std::string> inoToStringMap;

#endif
