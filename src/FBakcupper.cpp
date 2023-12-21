#include "parser.h"
#include "BackupHelper.h"
#include "RestoreHelper.h"

int main(int argc, char **argv){

    if(argc < 2){
        Usage();
        exit(0);
    }

    
    Paras paras;
    doParaParser(argc, argv, paras);

    if(paras.backup){
        // specify output_dir
        initConfig(paras);

        BackupHelper b_helper(paras);

        b_helper.doTask();
    
        // b_helper.getrepath();
        // TODO: 压缩，加密
    }
    else if(paras.restore){
        // TODO: 解压，解密
    
        RestoreHelper r_helper(paras);

        r_helper.doTask();
        
    }
    else if(paras.list){

        initConfig(paras);

        ListHelper l_helper(paras);

        l_helper.doTask();
    }
}

