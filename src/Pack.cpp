#include "Pack.h"

Pack::Pack(const File &file, std::filesystem::path backPath){
    this->header.name = file.name;
    this->header.metadata = file.metadata;

    this->src_filePath = backPath;
}

Pack::Pack(Header &rec_header){

}

void Pack::write_one_bkfile_into(std::string dest_filepath){
    std::ofstream bk_file(dest_filepath, std::ios::out | std::ios::app);

    if(!bk_file.is_open()){
        exit(-1);
    }

    Header real_header;
    strncpy(real_header.name, this->header.name.c_str(), sizeof(real_header.name) - 1);
    real_header.metadata = this->header.metadata;
    // if(S_ISLNK(this->header.metadata.st_mode)){
        
    // }

    bk_file.write(reinterpret_cast<const char*>(& real_header), sizeof(real_header));
    bk_file.close();

    if(S_ISREG(header.metadata.st_mode)){
        std::ifstream sourceFile(this->src_filePath, std::ios::binary);
        std::ofstream bk_file(dest_filepath, std::ios::out | std::ios::binary | std::ios::app);
        
        char byte;
        while (sourceFile.get(byte)) {
            bk_file.put(byte);
        }

        sourceFile.close();
        bk_file.close();
    }
    // TODO: 处理其他文件的数据块

    return;
}