#ifndef INCLUDE_RESTOREHELPER
#define INCLUDE_RESTOREHELPER

#include "parser.h"
#include "Pack.h"
#include "TaskHelper.h"
#include "ZIP_UNZIP.h"

class RestoreHelper : public TaskHelper{
private:
    // 文件名
    std::string input_name;
    // 文件路径
    std::filesystem::path Targetfile_Path;

public:
    RestoreHelper(const Paras &p);
    void doTask();
    void doUnpack(std::filesystem::path &destPath);
    void doUncompress();
};

std::filesystem::path removeBytes(const std::filesystem::path& filePath, std::size_t numBytesToRemove);
#endif