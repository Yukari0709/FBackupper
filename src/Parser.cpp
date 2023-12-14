#include "parser.h"

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
    std::cout << "  -b, --backup        Do File Backup.\n";
    std::cout << "  -r, --restore       Do File Restore.\n";
    std::cout << "  -l, --list          List all the backupped files.\n";
    std::cout << "  -f, --files         Specify the path of files(or directory) to be backed up or restore.\n";
    std::cout << "      --path          View the path where the backup files are stored/\n";
    std::cout << "  -c, --compress      Compress backup packages.\n";
    std::cout << "  -e, --encrypt       Encrypt backup packages, must be used with -p!\n";
    std::cout << "  -p, --password      Specify the compression password.\n";
    std::cout << "\nOptions to filter backup files:\n";
    std::cout << "      --name exp      Select files using regular expressions and their names, like \'*.o\' .\n";
    std::cout << "      --size t1 t2    Select files within a specified size range, like \'1000 2000\'(Bytes).\n";
    std::cout << "      --time t1 t2    Select files within a specified creation time range, like \'2023-12-11-20:31:12 2023-12-11-20:30:00\'.\n";
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
    }else{
        // Backup files database.
        std::filesystem::path backfilesPath = curPath / "Backfiles";
        std::filesystem::create_directory(backfilesPath);

        std::ofstream configFile(configPath);
        if (configFile.is_open()) {
            configFile << "PATH = " << backfilesPath << std::endl;
            configFile.close();
        } else {
            std::cerr << "Error creating Config.ini." << std::endl;
            exit(0);
        }

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
    
    app.add_option("-p,--password", paras.password);
    app.add_option("--name", paras.re_name);
    app.add_option("--size", paras.size);
    app.add_option("--time", paras.time);
    

    CLI11_PARSE(app, argc, argv);

    // TODO: check

    if(paras.backup && paras.restore){
        std::cout<<"[!] Bad parameters, can not backup and restore at the same time. \n";
        Usage();
        exit(0);
    }
    
    if(paras.backup || paras.restore){
        if(paras.input_path.empty()){
            std::cout<<"[!] Bad parameters, please specify the input path and the output path. \n";
            Usage();
            exit(0);
        }
    }

    return;
}