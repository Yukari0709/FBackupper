#include "parser.h"

std::filesystem::path changeRELtoABS1(std::filesystem::path &path, char *rel_path){

    // 如果rel_path已经是绝对路径，不进行转换
    std::string str(rel_path);
    if(str.front() != '.'){
        std::filesystem::path tmp1(str);
        return tmp1;
    }

    std::istringstream iss(rel_path);
    std::string token;

    // 对这个场景的适配
    std::filesystem::path tmp = path;
    
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

std::string GetPathFromConfig(const std::string& configFilename) {
    std::ifstream configFile(configFilename);
    std::string line;
    std::string key = "PATH";
    std::string pathValue;

    while (std::getline(configFile, line)) {
        std::size_t delimiterPos = line.find('=');

        if (delimiterPos != std::string::npos) {
            std::string currentKey = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            currentKey.erase(currentKey.find_last_not_of(" \t") + 1);
            currentKey.erase(0, currentKey.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));

            if (currentKey == key) {
                pathValue = value;
                break;
            }
        }
    }

    return pathValue;
}

void Usage(void){
    std::cout << "Usage: FBackupper [Options] [Paras] ... \n\nOptions:\n";
    // std::cout << "  -h, --help          Display this help message.\n";
    std::cout << "  -b, --backup            Do File Backup.\n";
    std::cout << "  -r, --restore           Do File Restore.\n";
    std::cout << "  -l, --list              List all the backupped files.\n";
    std::cout << "  -i, --input         Specify the path of files(or directory) to be backed up or restore.\n";
    std::cout << "  -o, --output         Specify where the path of RESTORE files.\n";
    // std::cout << "      --path              View the path where the backup files are stored/\n";
    std::cout << "  -c, --compress          Compress backup packages.\n";
    std::cout << "  -e, --encrypt           Encrypt backup packages, must be used with -p!\n";
    std::cout << "  -p, --password          Specify the compression password.\n";
    std::cout << "\nOptions to filter backup files:\n";
    std::cout << "      --repath ReguExp    Select files using regular expressions and their names, like \'*.o\' .\n";
    std::cout << "      --rename ReguExp    Select files using regular expressions and their names, like \'*.o\' .\n";
    std::cout << "      --size t1 t2        Select files within a specified size range, like \'1000 2000\'(Bytes).\n";
    std::cout << "      --ctime t1 t2       Select files within a specified last changed time range, like \'2023-12-11-20:31:12 2023-12-11-20:30:00\'.\n";
    std::cout << "      --mtime t1 t2       Select files within a specified last modified time range, like \'2023-12-11-20:31:12 2023-12-11-20:30:00\'.\n";
    std::cout << "      --type n1 n2 ...    Select types of file to be back upped: 1 -> Regular file; 2 -> Directory; 3 -> Symbolic link; 4 -> FIFO or pipe.\n";
}

void initConfig(Paras &paras){
    char runPath[256] = {0};
    getcwd(runPath, sizeof(runPath));
    std::filesystem::path curPath(runPath);
    std::filesystem::path configPath = curPath / "Config.ini";

    if (std::filesystem::exists(configPath)) {
        paras.output_path = GetPathFromConfig(configPath);

        if(paras.output_path.empty()){
            std::cout << "Bad Config file.\n";
            exit(0);
        }

        if(!std::filesystem::exists(paras.output_path)){
            if(!std::filesystem::create_directory(paras.output_path)){
                std::cout << "[!] Can not create_directory.\n ";
                exit(-1);
            }
        }
    }else{
        // Backup files database.
        std::filesystem::path backfilesPath = curPath / "Backfiles";
        if(!std::filesystem::exists(backfilesPath)){
            if(!std::filesystem::create_directory(backfilesPath)){
                std::cout << "[!] Can not create_directory.\n ";
                exit(-1);
            }
        }

        std::ofstream configFile(configPath);
        if (configFile.is_open()) {
            configFile << "PATH = " << backfilesPath.string() << std::endl;
            configFile.close();
        } else {
            std::cerr << "Error creating Config.ini." << std::endl;
            exit(0);
        }

        paras.output_path = backfilesPath.string();

    }
}

void doParaParser(int argc, char **argv, Paras &paras){
    CLI::App app{"FBackupper"};

    app.add_flag("-b,--backup", paras.backup, "Backup");
    app.add_flag("-r,--restore", paras.restore, "Restore");
    app.add_flag("-l,--list", paras.restore, "List");
    app.add_option("-f,--files", paras.input_path, "Specify the path of files(or directory) to be backed up or restore.");
    app.add_flag("-c,--compress", paras.compress);
    app.add_flag("-e,--encrypt", paras.encrypt);
    app.add_option("-i,--input", paras.input_path);
    app.add_option("-o,--output", paras.rec_path);
    
    app.add_option("-p,--password", paras.password);
    app.add_option("--rename", paras.re_name);
    app.add_option("--repath", paras.re_path);
    app.add_option("--size", paras.size);
    app.add_option("--ctime", paras.ctime);
    app.add_option("--mtime", paras.mtime);
    
    
    app.add_option("--type", paras.typenum);

    CLI11_PARSE(app, argc, argv);

    // TODO: check

    if(paras.backup && paras.restore){
        std::cout<<"[!] Bad parameters, can not backup and restore at the same time. \n";
        exit(0);
    }
    
    if(paras.backup || paras.restore){
        if(paras.input_path.empty()){
            std::cout<<"[!] Bad parameters, please specify the input path and the output path. \n";
            exit(0);
        }
    }

    // 处理输入的path
    if(paras.backup){
        if(paras.input_path.back() == '/'){
            paras.input_path.pop_back();
        }
        // 将相对路径转换为绝对
        std::filesystem::path currentPath = std::filesystem::current_path();

        std::filesystem::path tmp = changeRELtoABS1(currentPath, paras.input_path.c_str());

        paras.input_path = tmp.string();
    }

    if(paras.restore){
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::filesystem::path tmp = changeRELtoABS1(currentPath, paras.output_path.c_str());

        paras.output_path = tmp.string();
    }

    return;
}

