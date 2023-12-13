#include "parser.h"
#include "BackupHelper.h"

int main(int argc, char **argv){

    if(argc < 2){
        Usage();
        exit(0);
    }

    initConfig();
    // TODO : parser
    doParaParser(argc, argv);

    BackupHelper b_helper(paras);

    if(paras.backup){

        // TODO: 设置备份参数，是否压缩，是否加密，加密密码。

        // TODO： 过滤文件，返回vector类型的文件列表。

        // b_helper.doBackup();
    
    }
}