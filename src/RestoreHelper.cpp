#include "RestoreHelper.h"

RestoreHelper::RestoreHelper(const Paras &p) : TaskHelper(p){
    this->input_name = p.input_path;
}

void RestoreHelper::doTask(){
    std::string BackfilesPath = this->global_paras.output_path;

    std::filesystem::path Backfiles_Path(BackfilesPath);
    std::filesystem::path ipnut_bkfile_name(this->input_name);

    // 待恢复的pack文件
    std::filesystem::path Targetfile_Path = Backfiles_Path / ipnut_bkfile_name;
    // 恢复到的path
    std::filesystem::path rec_Path(this->global_paras.rec_path);

    if(access(Targetfile_Path.c_str(), R_OK) != 0){
        std::cerr << "[!] Can not Read from bk file: " << Targetfile_Path << std::endl;
        exit(-1);
    }

    // TODO: 读文件头分析

    std::ifstream bkfile(Targetfile_Path, std::ios::binary);
    if (!bkfile.is_open()) {
        std::cerr << "[!] Error opening file: " << Targetfile_Path << std::endl;
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