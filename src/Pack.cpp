#include "Pack.h"
// 恢复hardlink文件，第一次出现加入map，后续出现创建hadlink
std::map<ino_t, std::string> rec_inomap;

Pack::Pack(const File &file, std::filesystem::path backPath){
    this->header.name = file.name;
    this->header.metadata = file.metadata;

    this->src_filePath = backPath;
}

Pack::Pack(Header &rec_header){
    this->restore_header = rec_header;
}

void Pack::write_one_bkfile_into(std::string dest_filepath){
    std::ofstream bk_file(dest_filepath, std::ios::out | std::ios::app);

    if(!bk_file.is_open()){
        exit(-1);
    }

    Header real_header;
    strncpy(real_header.name, this->header.name.c_str(), sizeof(real_header.name) - 1);
    real_header.metadata = this->header.metadata;

    // 写入header
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

    return;
}

void Pack::write_one_bkfile_into(std::string dest_filepath, char * target){
    std::ofstream bk_file(dest_filepath, std::ios::out | std::ios::app);

    if(!bk_file.is_open()){
        exit(-1);
    }

    Header real_header;
    strncpy(real_header.name, this->header.name.c_str(), sizeof(real_header.name) - 1);
    real_header.metadata = this->header.metadata;
    strncpy(real_header.link_target, target, sizeof(real_header.link_target) - 1);

    // 写入header
    bk_file.write(reinterpret_cast<const char*>(& real_header), sizeof(real_header));
    bk_file.close();

}

void Pack::restore_from_header(std::filesystem::path &path, std::ifstream& inputFile){
    // 根据一个文件头恢复一个文件到path
    if(this->restore_header.magicNumber != HEADER_MAGIC_NUMBER){
        std::cerr << "[!] ERROR: Unexpected magic number. \n" ;
        exit(-1);
    }

    std::filesystem::path relative_path = std::filesystem::u8path(restore_header.name);
    std::filesystem::path absolute_path = path / relative_path;

    std::cout << "[*] Restoring file: " << restore_header.name << std::endl;

    if(std::filesystem::exists(path / relative_path)){
        // 已存在？
        std::cout << "[!] Target path already exist.\n";
        exit(0);
    }
    
    mode_t fileType = restore_header.metadata.st_mode & S_IFMT;

    if(fileType == S_IFDIR){
        // dir文件
        std::filesystem::create_directory(absolute_path);
    }
    else if(restore_header.metadata.st_nlink > 1){
        // hardlink文件, 必须存在源文件才能创建hdlink。根据我们的filter一定是先还原源文件再还原hdlink.
        if(restore_header.link_target[0] == '\0'){
            goto HANDLE_REGFILE;
        }
        else{
            std::filesystem::path hdl(restore_header.link_target);
            std::filesystem::path tmp = path / hdl;
            std::filesystem::create_hard_link(tmp, absolute_path);
        }
    }
    else if(fileType == S_IFLNK){
        // symlink文件
        std::filesystem::create_symlink(restore_header.link_target, absolute_path);
    }
    else if(fileType == S_IFREG){
HANDLE_REGFILE:
        // 普通文件
        off_t size = restore_header.metadata.st_size;
        
        std::ofstream des_File(absolute_path, std::ios::binary | std::ios::trunc);
        if (!des_File.is_open()) {
            std::cerr << "[!] Error opening destination file: " << absolute_path << std::endl;
            return;
        }

        char byte;
        for (off_t i = 0; i < size; ++i) {
            if (inputFile.get(byte)) {
                des_File.put(byte);
            } else {
                std::cerr << "[!] Error reading from inputFile or end of file reached." << std::endl;
                break;
            }
        }
    }

    // TODO: 还原文件的metadata
    struct stat meta = this->restore_header.metadata;
    
    chmod(absolute_path.c_str(), meta.st_mode);
    lchown(absolute_path.c_str(), meta.st_uid, meta.st_gid);

    timespec time_meta[2] = {meta.st_atim, meta.st_mtim};
    utimensat(AT_FDCWD, absolute_path.c_str(), time_meta, AT_SYMLINK_NOFOLLOW);

}