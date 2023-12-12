#include "parser.h"

int main(int argc, char **argv){

    if(argc < 2){
        Usage();
    }

    // TODO : parser
    doParaParser(argc, argv);

    // if(备份){
    //     if(还原) 报错；
        
    //     files = filter();

    //     out_file = pack(files);

    //     if(压缩){
    //         out_file = docompress(out_file);
    //     }
    //     if(加密){
    //         out_file = doencrypt(out_file);
    //     }

    //     out_file = metadate(out_file);

    //     return;
    // }
}