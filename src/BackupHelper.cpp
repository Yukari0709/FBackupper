#include "BackupHelper.h"

void BackupHelper::doBackup(std::vector<File> file_names, std::string input_path){
    // TODO：
}

BackupHelper::BackupHelper(const Paras &p): _compress(p.compress), _encrypt(p.encrypt), re_name(p.re_name), size(p.size), time(p.time){
    
}

void BackupHelper::doFilter(){
    
}