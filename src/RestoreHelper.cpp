#include "RestoreHelper.h"

RestoreHelper::RestoreHelper(const Paras &p) : TaskHelper(p){
    this->input_name = p.input_path;

    std::string BackfilesPath = this->global_paras.output_path;
    std::filesystem::path Backfiles_Path(BackfilesPath);

    std::filesystem::path ipnut_bkfile_name(this->input_name);
    this->Targetfile_Path = Backfiles_Path / ipnut_bkfile_name;

}

void RestoreHelper::doTask(){
    // TODO: 读文件头解析
    header_for_whole_file header;
    std::ifstream inFile(Targetfile_Path, std::ios::binary);
    inFile.read(reinterpret_cast<char*>(&header), sizeof(header));
    inFile.close();

    // 创建没有文件头的临时文件
    std::filesystem::path tmp = removeBytes(Targetfile_Path, 216);

    // uncompress
    if(header.encrypt){
        
    }

    if(header.compress){
        std::string tmp_uncompress = tmp.string() + ".uncom";
        huffman::unzip(tmp.string(), tmp_uncompress);
        // 删除未解压的tmp
        std::filesystem::remove(tmp);
        tmp = tmp_uncompress;
    }
    
    // unpack
    doUnpack(tmp);
    
    // 删除临时文件
    std::filesystem::remove(tmp);
}

void RestoreHelper::doUnpack(std::filesystem::path &destPath){

    // 恢复到的path
    std::filesystem::path rec_Path(this->global_paras.rec_path);

    if(access(destPath.c_str(), R_OK) != 0){
        std::cerr << "[!] Can not Read from bk file: " << destPath << std::endl;
        exit(-1);
    }

    std::ifstream bkfile(destPath, std::ios::binary);

    if (!bkfile.is_open()) {
        std::cerr << "[!] Error opening file: " << destPath << std::endl;
        exit(-1);
    }
    
    Header header;

    while(bkfile.read(reinterpret_cast<char*>(&header), sizeof(Header))){

        // bkfile.read(reinterpret_cast<char*>(&header), sizeof(Header));

        Pack pack(header);
        pack.restore_from_header(rec_Path, bkfile);
    }

    bkfile.close();

    std::cout << "[+] Finish restore to dir: "<< rec_Path << std::endl;
}

std::filesystem::path removeBytes(const std::filesystem::path& filePath, std::size_t numBytesToRemove) {
    std::ifstream inputFile(filePath, std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return;
    }

    inputFile.seekg(0, std::ios::end);
    std::size_t fileSize = static_cast<std::size_t>(inputFile.tellg());
    inputFile.seekg(0, std::ios::beg);

    if (numBytesToRemove >= fileSize) {
        std::cerr << "Error: Number of bytes to remove is greater than or equal to the file size." << std::endl;
        return;
    }

    std::filesystem::path tempFilePath = filePath;
    tempFilePath += ".tmp";
    std::ofstream tempFile(tempFilePath, std::ios::binary);

    if (!tempFile.is_open()) {
        std::cerr << "Error: Unable to create temporary file " << tempFilePath << std::endl;
        return;
    }

    inputFile.seekg(numBytesToRemove, std::ios::beg);

    tempFile << inputFile.rdbuf();
    inputFile.close();
    tempFile.close();

    return tempFilePath;
}
