#include "parser.h"
// definition
Paras paras;

void Usage(void){
    std::cout << "Usage: FBackupper [Options] [Paras] ... \n\nOptions:\n";
    std::cout << "  -h, --help          Display this help message.\n";
    std::cout << "  -b, --backup        Do File Backup.\n";
    std::cout << "  -r, --restore       Do File Restore.\n";
    std::cout << "  -i, --input         Specify the path of files(or directory) to be backed up.\n";
    std::cout << "  -o, --output        Specify where to store your backup files.\n";
    std::cout << "  -c, --compress      Compress backup packages.\n";
    std::cout << "  -e, --encrypt       Encrypt backup packages, must be used with -p!\n";
    std::cout << "  -p, --password      Specify the compression password.\n";
    std::cout << "\nOptions to filter backup files:\n";
    std::cout << "      --name exp      Select files using regular expressions and their names, like \'*.o\' .\n";
    std::cout << "      --size t1 t2    Select files within a specified size range, like \'1000 2000\'(Bytes).\n";
    std::cout << "      --time t1 t2    Select files within a specified creation time range, like \'2023-12-11 20:31:12 2023-12-12 20:30:00\'.\n";
}

void doParaParser(int argc, char **argv){
    CLI::App app{"FBackupper"};

    app.add_option("-b,--backup", paras.backup, "Backup");
    app.add_option("-r,--restore", paras.restore, "res");

    CLI11_PARSE(app, argc, argv);

    // TODO: check
    return;
}