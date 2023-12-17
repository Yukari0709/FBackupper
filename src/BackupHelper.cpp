#include "BackupHelper.h"

void BackupHelper::doBackup(){
    // TODO：
    // doFilter();
    if(all_files.empty());
    
    // 创建新文件
    initBackupFile();
    // 打包

    // doPack();

    // 压缩

    // 加密
}

BackupHelper::BackupHelper(const Paras &p): _compress(p.compress), _encrypt(p.encrypt), re_name(p.re_name), size(p.size), ctime(p.ctime), mtime(p.mtime), passwd(p.password){
    input_path = p.input_path;
    output_path = p.output_path;
}

void BackupHelper::doFilter(){
    processPath(input_path);
    for (const auto &element : all_files){
        std::cout << element.name << " ";
    }
    if(!ctime.empty()){
        for (auto it = all_files.begin(); it != all_files.end(); ) {
        bool flag_ctime = checkFilesCreationTime(it->metadata);
        if (flag_ctime) {
            ++it;  
        } else {
            it = all_files.erase(it); 
        }
     }
    }
    
    for (const auto &element : all_files){
        std::cout << element.name << " ";
    }
}

void BackupHelper::processPath(const std::string& current_path) {
   try {
            // 处理给定路径的逻辑
            struct stat current_metadata;
            if (stat(current_path.c_str(), &current_metadata) == 0) {
                File current_file;
                current_file.name = std::filesystem::path(current_path).filename(); // 获取给定路径的文件名
                current_file.metadata = current_metadata;
                all_files.push_back(current_file);
            } else {
                std::cerr << "无法获取给定路径的文件信息：" << current_path << std::endl;
            }

            // 遍历给定路径下的文件和子目录
            for (const auto& entry : std::filesystem::recursive_directory_iterator(current_path)) {
                std::string entry_path = entry.path().string();
                std::string relative_path = std::filesystem::path(current_path).filename().string() + '/' + entry.path().lexically_relative(current_path).string();
                // 创建 File 结构体
                struct stat file_metadata;
                if (stat(entry_path.c_str(), &file_metadata) == 0) {
                    File file;
                    file.name = relative_path;
                    file.metadata = file_metadata;
                    all_files.push_back(file);
                } else {
                    std::cerr << "无法获取文件信息：" << entry_path << std::endl;
                }
            }
        } catch (const std::exception& ex) {
            std::cerr << "遍历路径时发生异常: " << ex.what() << std::endl;
        }
    }


std::time_t convertStringToTime(const std::string& str_time) {
    struct tm tm;
    strptime(str_time.c_str(), "%Y-%m-%d-%H:%M:%S", &tm); // 修改时间格式
    return std::mktime(&tm);
}

bool BackupHelper::checkFilesCreationTime(const struct stat& metadata){
    std::time_t creation_time = metadata.st_ctim.tv_sec;
    std::time_t t1 = convertStringToTime(ctime[0]);
    std::time_t t2 = convertStringToTime(ctime[1]);
    if(creation_time >= t1 && creation_time <= t2){
        return true;
    }
    else{
        return false;
    }
}



bool BackupHelper::checkFilesModifyTime(const struct stat& metadata){

}

bool BackupHelper::checkFilesSize(const struct stat& metadata){

}

bool BackupHelper::checkFilesAddress(const struct stat& metadata){

}

bool BackupHelper::checkFilesName(const struct stat& metadata){

}

bool BackupHelper::checkFilesType(const struct stat& metadata){

}

std::string generateFileName(void){
    auto currentTime = std::chrono::system_clock::now();

    // 将时间点转换为 time_t
    std::time_t time = std::chrono::system_clock::to_time_t(currentTime);

    // 使用 std::put_time 将 time_t 格式化为字符串
    std::tm timeInfo = *std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(&timeInfo, "%Y-%m-%d-%H:%M:%S");
    return ss.str();
}

void BackupHelper::initBackupFile(){
    if(output_path.empty()){
        std::cout << "[!] ERROR: Can not get output dir.\n";
        exit(-1);
    }

    std::string tmpfilename = generateFileName();
    tmpfilename = output_path + tmpfilename + ".bup";

    std::ofstream outputFile(tmpfilename);
    if (!outputFile.is_open()) {
        std::cout << "[!] Can not create new file. \n";
        exit(-1);
    }

    outputFile.close();

    // TODO: 写入压缩文件的头


}

void BackupHelper::getctime(){
    for(const auto &ctime : ctime){
        std::cout << ctime << std::endl;
    }
}

void BackupHelper::getctimetrans(){
    for(const auto &ctime : ctime){
        std::time_t t = convertStringToTime(ctime);
        std::cout << t << std::endl;
    }
}