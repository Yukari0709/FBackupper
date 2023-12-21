#ifndef INCLUDE_PACK
#define INCLUDE_PACK

#include "file.h"
#include "parser.h"

struct Header
{
    uint32_t magicNumber;
    char name[100];
    struct stat metadata;
    char link_target[100];

    Header(): magicNumber(HEADER_MAGIC_NUMBER), name{}, link_target{} {}
}; // 352 byte header

class Pack{
private:
    // src_filePath为待备份文件的绝对路径
    std::string src_filePath;

    Header_for_one_file header;
    std::string LNK_Target;
    
    // 恢复的时候读取到的pack头
    Header restore_header;

public:
    // Use when backup
    Pack(const File &file, std::filesystem::path backPath);
    // Use when recovery
    Pack(Header &rec_header);

    void write_one_bkfile_into(std::string dest_filepath);
    // Use when back link files
    void write_one_bkfile_into(std::string dest_filepath, char * target);

    // Use when restore file from a header.
    void restore_from_header(std::filesystem::path &path, std::ifstream& inputFile);
};

extern std::map<ino_t, std::string> rec_inomap;

#endif