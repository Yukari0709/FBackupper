#include "BackupHelper.h"

std::map<ino_t, std::string> inoToStringMap;

void BackupHelper::doBackup(){
    // TODO：
    // doFilter();
    if(all_files.empty());
    
    // 创建新文件
    initBackupFile();
    
    // 打包
    doPack();

    // 压缩

    // 加密
}

BackupHelper::BackupHelper(const Paras &p): _compress(p.compress), _encrypt(p.encrypt), re_name(p.re_name), size(p.size), ctime(p.ctime), mtime(p.mtime), passwd(p.password){
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
        std::cerr << "处理路径时发生异常1234: " << ex.what() << std::endl;
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
    tmpfilename = output_path + "/" + tmpfilename + ".bup";

    // 创建新文件
    std::ofstream outputFile(tmpfilename);
    if (!outputFile.is_open()) {
        std::cout << "[!] Can not create new file. \n";
        exit(-1);
    }

    outputFile.close();

    this->bkfile_path = tmpfilename;

    // TODO: 写入压缩文件的头

}

void BackupHelper::doPack(){

    // For develop
    File tmp0; // 第一个文件一定是代表文件夹的特殊文件
    std::string path_to_test = "/home/jiangshao/Filebackup";
    tmp0.name = "test";
    if (stat(this->input_path.c_str(), &tmp0.metadata) != 0){
        std::cout << "[!] ERROR: Can not get metadata for 123. \n";
        exit(-1);
    }
    this->all_files.push_back(tmp0);

    File tmp;
    tmp.name = "test/123";
    std::string tmp_path = path_to_test + '/' + tmp.name;
    if (stat(tmp_path.c_str(), &tmp.metadata) != 0){
        std::cout << "[!] ERROR: Can not get metadata for 123. \n";
        exit(-1);
    }
    this->all_files.push_back(tmp);

    File tmp1;
    tmp1.name = "test/456";
    tmp_path = path_to_test + '/' + tmp1.name;
    if (stat(tmp_path.c_str(), &tmp1.metadata) != 0){
        std::cout << "[!] ERROR: Can not get metadata for 123. \n";
        exit(-1);
    }
    this->all_files.push_back(tmp1);

    File tmp2;
    tmp2.name = "test/sym_link";
    tmp_path = path_to_test + '/' + tmp2.name;
    if (stat(tmp_path.c_str(), &tmp2.metadata) != 0){
        std::cout << "[!] ERROR: Can not get metadata for 123. \n";
        exit(-1);
    }    
    this->all_files.push_back(tmp2);
    // For develop

    // check output file
    if(access(bkfile_path.c_str(), W_OK) != 0){
        std::cerr << "[!] Can not write to bk file. \n";
        exit(-1);
    }

    std::filesystem::path i_path = this->input_path;
    std::filesystem::path parentPath = i_path.parent_path();

    for(const File &elem : all_files){
        // check whether file can be readed
        // input_path和name有重合的地方，需要处理一下

        std::filesystem::path backPath = parentPath / elem.name;

        if (std::filesystem::is_symlink(backPath)) {
            // 处理sym link文件
            char targetPath[100];
            ssize_t bytesRead = readlink(backPath.c_str(), targetPath, 100 - 1);
            if (bytesRead != -1) {
                targetPath[bytesRead] = '\0';
            }

            // 如果源文件也要备份，那么先备份源文件。

            // elem的metadata要更新，用lstat重新获取
            lstat(backPath.c_str(), &elem.metadata);
            Pack pack(elem, backPath);
            pack.write_one_bkfile_into(bkfile_path, targetPath);

        }
        else if(elem.metadata.st_nlink > 1){
            // 处理hard link文件
            ino_t linkedInode = elem.metadata.st_ino;
            auto it = inoToStringMap.find(linkedInode);

            if (it != inoToStringMap.end()) {
                const char * link_t = it->second.c_str();

                Pack pack(elem, backPath);
                pack.write_one_bkfile_into(bkfile_path, link_t);
            } else{

                inoToStringMap[linkedInode] = elem.name;

                Pack pack(elem, backPath);
                pack.write_one_bkfile_into(bkfile_path);
            }
        }
        else if(S_ISREG(elem.metadata.st_mode)){

            if(access(backPath.c_str(), R_OK) != 0){
                std::cerr << "[!] Can not read from file : " << elem.name << " .\n";
                exit(-1);
            }
            Pack pack(elem, backPath);
            pack.write_one_bkfile_into(bkfile_path);
        }
    }

}