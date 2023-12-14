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

BackupHelper::BackupHelper(const Paras &p): _compress(p.compress), _encrypt(p.encrypt), re_name(p.re_name), size(p.size), time(p.time), passwd(p.password){
    input_path = p.input_path;
    output_path = p.output_path;
}

void BackupHelper::doFilter(){

}

void BackupHelper::processPath(const std::string& current_path) {
    namespace fs = std::filesystem;

    try {
        if (fs::is_directory(current_path)) {
            // 如果是目录，则遍历目录下的文件
            for (const auto& entry : fs::directory_iterator(current_path)) {
                processPath(entry.path().string());
            }
        } else if (fs::is_regular_file(current_path)) {
            // 如果是文件，则将文件添加到向量中
            File file;
            file.name = current_path;
            if (stat(current_path.c_str(), &file.metadata) == 0) {
                all_files.push_back(file);
            } else {
                std::cerr << "无法获取文件信息：" << current_path << std::endl;
            }
        } else {
            std::cerr << "无效的路径或文件：" << current_path << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "处理路径时发生异常: " << ex.what() << std::endl;
    }
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