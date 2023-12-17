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
}; // 348 byte header

class Pack{
private:
    // src_filePath为待备份文件的绝对路径
    std::string src_filePath;
    Header_for_one_file header;

    void get_file_data();

public:
    // Use when backup
    Pack(const File &file, std::filesystem::path backPath);
    // Use when recovery
    Pack(Header &rec_header);

    void write_one_bkfile_into(std::string dest_filepath);

};

#endif