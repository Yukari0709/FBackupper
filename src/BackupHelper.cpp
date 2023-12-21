#include "BackupHelper.h"
#include <regex>
#include <algorithm>

std::map<ino_t, std::string> inoToStringMap;

void writeHeader(std::string src, std::string dest, header_for_whole_file &header){
    std::ofstream outFile(dest, std::ios::out | std::ios::binary | std::ios::trunc);
    outFile.write(reinterpret_cast<char*>(&header), sizeof(header));

    std::ifstream originalFile(src, std::ios::binary);

    char byte;
    while (originalFile.get(byte)) {
        outFile.put(byte);
    }

    outFile.close();
    originalFile.close();

    std::filesystem::remove(src);
    std::filesystem::rename(dest, dest.substr(0, dest.size() - 1));
}

void BackupHelper::doTask(){

    // 自定义过滤
    Filter filter(global_paras);
    filter.doTask();
    this->all_files = filter.getFiles();

    if(all_files.empty() | all_files.size() == 1){
        std::cout << "[!] Bad parameter, please check your input path.\n";
        exit(-1);
    }

    // 创建新文件
    initBackupFile();

    // 打包
    doPack();

    std::cout << "[+] Finish packing into file: " << this->bkfile_path << std::endl;

    // 压缩
    if(global_paras.compress){
        // using namespace huffman;
        std::string desFilePath = this->bkfile_path + ".com";

        huffman::compression(this->bkfile_path, desFilePath);

        std::cout << "[+] Finish compressing into file: " << desFilePath << std::endl;

        try {
            std::filesystem::remove(this->bkfile_path);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "[!] Error deleting file: " << e.what() << std::endl;
            return -1;
        }

        this->bkfile_path = desFilePath;
    }

    // 加密
    if(global_paras.encrypt){

    }

    // 写入文件头
    header_for_whole_file header;

    header.cre_time = std::time(nullptr);
    if(!this->global_paras.comment.empty()){
        strncpy(header.comment, this->global_paras.comment.c_str(), 100 - 1);
    }
    strncpy(header.srcPath, global_paras.input_path.c_str(), 100 - 1);
    header.compress = global_paras.compress;
    header.encrypt = global_paras.encrypt;

    std::string tmp_out = this->bkfile_path + 't';
    writeHeader(this->bkfile_path, tmp_out, header);
}

BackupHelper::BackupHelper(const Paras &p): TaskHelper(p) {

}

std::string getAbsolutePath(const std::string& fileName, const std::string& currentPath) {
    size_t lastSlashPos = currentPath.find_last_of('/');
    std::string ab_path;
    if (lastSlashPos != std::string::npos) {
        ab_path =  currentPath.substr(0, lastSlashPos);
    } else {
        // 如果没有找到斜杠，返回原始字符串
        return currentPath;
    }
    ab_path = ab_path + '/' + fileName;
   // std::cout << ab_path << std::endl;
    return ab_path;
}

int getFileType(const std::string& path){
    struct stat fileStat;
    //首先判断给定的目录文件是否能获取文件信息
    if (lstat(path.c_str(), &fileStat) != 0) {
        perror("Error getting file status");
        return -1;
    }

    if (S_ISFIFO(fileStat.st_mode)) {
        // 管道文件
        return 4;
    } else if (S_ISDIR(fileStat.st_mode)) {
        // 目录文件
        return 2;
    } else {
        // 不是管道也不是目录，进一步判断软链接
        if (S_ISLNK(fileStat.st_mode)) {
            // 软链接
            return 3;
        } else {
            // 不是软链接，再判断是否为硬链接
            if (fileStat.st_nlink > 1) {
                // 硬链接
                return 3;
            } else if (S_ISREG(fileStat.st_mode)) {
                // 常规文件
                return 1;
            } else {
                // 其他类型
                return -1;
            }
        }
    }
}
    

bool findNumber(int target, const std::vector<int>& typenum) {
    auto it = std::find(typenum.begin(), typenum.end(), target);
    return it != typenum.end();
}

std::time_t convertStringToTime(const std::string& str_time) {
    struct tm tm;
    strptime(str_time.c_str(), "%Y-%m-%d-%H:%M:%S", &tm); // 修改时间格式
    return std::mktime(&tm);
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
    if(global_paras.output_path.empty()){
        std::cout << "[!] ERROR: Can not get output dir.\n";
        exit(-1);
    }

    std::string tmpfilename = generateFileName();
    tmpfilename = global_paras.output_path + "/" + tmpfilename + ".bup";

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

std::filesystem::path changeRELtoABS(std::filesystem::path &path, char *rel_path){

    // 如果rel_path已经是绝对路径，不进行转换
    std::string str(rel_path);
    if(str.front() != '.'){
        std::filesystem::path tmp1(str);
        return tmp1;
    }

    std::istringstream iss(rel_path);
    std::string token;

    std::filesystem::path tmp = path.parent_path();
    
    while (std::getline(iss, token, '/')) {
        if(token == "."){
            tmp = tmp;
        }
        else if(token == ".."){
            tmp = tmp.parent_path();
        }
        else{
            tmp /= token;
        };
    }

    return tmp;
}

void BackupHelper::doPack(){

    // check output file
    if(access(bkfile_path.c_str(), W_OK) != 0){
        std::cerr << "[!] Can not write to bk file. \n";
        exit(-1);
    }

    std::filesystem::path i_path = this->global_paras.input_path;
    std::filesystem::path parentPath = i_path.parent_path();

    for(const File &elem : all_files){

        std::cout << "[*] Packing file: " << elem.name << std::endl;

        std::filesystem::path backPath = parentPath / elem.name;

        if (std::filesystem::is_symlink(backPath)) {
            // 处理sym link文件
            char targetPath[100];
            // 这里可能获得相对路径，比如./，我们需要转换为绝对路径
            ssize_t bytesRead = readlink(backPath.c_str(), targetPath, 100 - 1);
            if (bytesRead != -1) {
                targetPath[bytesRead] = '\0';
            }

            // 相对路径转换为绝对路径，备份symlink的思路是保持链接对象
            std::filesystem::path abs_p = changeRELtoABS(backPath, targetPath);
            // std::cout << abs_p << std::endl;

            // elem的metadata要更新，用lstat重新获取
            lstat(backPath.c_str(), &elem.metadata);
            Pack pack(elem, backPath);
            pack.write_one_bkfile_into(bkfile_path, abs_p.c_str());
        }
        else if(elem.metadata.st_nlink > 1){
            // 处理hard link文件
            ino_t linkedInode = elem.metadata.st_ino;
            auto it = inoToStringMap.find(linkedInode);

            if (it != inoToStringMap.end()) {
                // 源文件已备份
                const char * link_t = it->second.c_str();

                Pack pack(elem, backPath);
                pack.write_one_bkfile_into(bkfile_path, link_t);
            } else{
                // 源文件未备份，该文件即为源文件
                inoToStringMap[linkedInode] = elem.name;

                // 注，默认该文件为普通文件，不处理嵌套链接
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

ListHelper::ListHelper(const Paras &p) : TaskHelper(p){

}

void ListHelper::doTask(){

    std::filesystem::path BKfile_path(this->global_paras.output_path);

    if(global_paras.input_path.empty()){
        std::cout << "[*] Backup file path: " << this->global_paras.output_path << std::endl;
        std::cout << "[*] All backup files: \n";
        try {
            for (const auto& entry : std::filesystem::directory_iterator(BKfile_path)) {
                std::cout << entry.path().filename() << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "[!] Error accessing the directory: " << ex.what() << std::endl;
            return 1;
        }

        std::cout << "\nUse -i [filename] to get more information of a backup file. \n";
        return;
    }
    // TODO: 根据选择的文件打印文件头信息
    std::filesystem::path file = BKfile_path / global_paras.input_path;
    
    header_for_whole_file header;
    std::ifstream inFile(file, std::ios::binary);
    inFile.read(reinterpret_cast<char*>(&header), sizeof(header));
    inFile.close();

    struct tm* timeInfo = std::localtime(&header.cre_time);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);

    std::cout << "Detailed information for back file: " << global_paras.input_path << std::endl;
    std::cout << "    Create time:            " << buffer << std::endl;
    std::cout << "    Origin directory Path:  " << header.srcPath << std::endl;
    std::cout << "    User comment:           " << header.comment << std::endl;
    std::cout << "    Backup mode:            ";
    if(header.compress){
        std::cout << "Compress(Huffman algorithm) ";
    }
    if(header.encrypt){

    }
    std::cout << std::endl;
}

Filter::Filter(const Paras &p) : TaskHelper(p) {

}

void Filter::doTask(){
    processPath(global_paras.input_path);

    if(!global_paras.re_path.empty()){
        for (auto it = std::next(all_files.begin()); it != all_files.end(); ) {
        bool flag_address = checkFilesAddress(it->name);
        if (flag_address) {
            ++it;  
        } else {
            it = all_files.erase(it); 
        }
     }
    }

    if(!global_paras.re_name.empty()){
        for (auto it = std::next(all_files.begin()); it != all_files.end(); ) {
        bool flag_name = checkFilesName(it->name);
        if (flag_name) {
            ++it;  
        } else {
            it = all_files.erase(it); 
        }
     }
    }

    if(!global_paras.typenum.empty()){
        for (auto it = std::next(all_files.begin()); it != all_files.end(); ) {
        bool flag_type = checkFilesType(it->name);
        if (flag_type) {
            ++it;  
        } else {
            it = all_files.erase(it); 
        }
     }
    }

    if(!global_paras.size.empty()){
        for (auto it = std::next(all_files.begin()); it != all_files.end(); ) {
        bool flag_size = checkFilesSize(it->metadata);
        if (flag_size) {
            ++it;  
        } else {
            it = all_files.erase(it); 
        }
     }
    }

    if(!global_paras.ctime.empty()){
        for (auto it = std::next(all_files.begin()); it != all_files.end(); ) {
        bool flag_ctime = checkFilesChangeTime(it->metadata);
        if (flag_ctime) {
            ++it;  
        } else {
            it = all_files.erase(it); 
        }
     }
    }
    
     if(!global_paras.mtime.empty()){
        for (auto it = std::next(all_files.begin()); it != all_files.end(); ) {
        bool flag_mtime = checkFilesModifyTime(it->metadata);
        if (flag_mtime) {
            ++it;  
        } else {
            it = all_files.erase(it); 
        }
     }
    }     
}

void Filter::processPath(const std::string& current_path){
    try {
        // 处理给定路径的逻辑
        struct stat current_metadata;
        if (stat(current_path.c_str(), &current_metadata) == 0) {
            File current_file;
            current_file.name = std::filesystem::path(current_path).filename(); // 获取给定路径的文件名
            current_file.metadata = current_metadata;
            all_files.push_back(current_file);
        } else {
            std::cerr << "[!] Unable to obtain file information for the given path: " << current_path << std::endl;
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
                std::cerr << "[!] Can not acquire file metadata: " << entry_path << std::endl;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[!] Error when go through path: " << ex.what() << std::endl;
    }
}

bool Filter::checkFilesChangeTime(const struct stat& metadata){
    std::time_t creation_time = metadata.st_ctim.tv_sec;
   // std::cout << creation_time << std::endl ;
    std::time_t t1 = convertStringToTime(global_paras.ctime[0]);
    std::time_t t2 = convertStringToTime(global_paras.ctime[1]);
    if(creation_time >= t1 && creation_time <= t2){
        return true;
    }
    else{
        return false;
    }
}



bool Filter::checkFilesModifyTime(const struct stat& metadata){
    std::time_t modify_time = metadata.st_mtim.tv_sec;
   // std::cout << modify_time << std::endl ;
    std::time_t t1 = convertStringToTime(global_paras.mtime[0]);
    std::time_t t2 = convertStringToTime(global_paras.mtime[1]);
    if(modify_time >= t1 && modify_time <= t2){
        return true;
    }
    else{
        return false;
    }
}

bool Filter::checkFilesSize(const struct stat& metadata){
    off_t f_size = metadata.st_size;
   // std::cout << "size value: " << f_size << std::endl;
    if(f_size >= global_paras.size[0] && f_size <= global_paras.size[1]){
        return true;
    }
    else{
        return false;
    }
}

bool Filter::checkFilesAddress(const std::string& name){
    std::regex regex_pattern(global_paras.re_path);
    return std::regex_match(name, regex_pattern);
}

bool Filter::checkFilesName(const std::string& name){
    // 构建正则表达式对象
    std::regex regex_pattern(global_paras.re_name);

    // 从相对路径/文件名中提取文件名
    size_t pos = name.find_last_of('/');
    std::string file_name = (pos != std::string::npos) ? name.substr(pos + 1) : name;
  // std::cout << file_name << std::endl;
    // 使用 regex_match 检查文件名是否与正则表达式匹配
    return std::regex_match(file_name, regex_pattern);
}


bool Filter::checkFilesType(const std::string &name){
    std::string file_abpath;
    file_abpath = getAbsolutePath(name, global_paras.input_path);
    int num;
    num = getFileType(file_abpath);
    //std::cout << num <<std::endl;
    return findNumber(num, global_paras.typenum);
}

std::vector<File> Filter::getFiles(){
    return this->all_files;
}