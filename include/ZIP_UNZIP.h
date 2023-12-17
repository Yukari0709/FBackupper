#ifndef _ZIP_UNZIP_H
#define _ZIP_UNZIP_H

#include <iostream>

#include "HuffmanEncode.h"
#include "HuffmanDecode.h"

using namespace huffman;

void compression(const std::string& srcfilename, const std::string& destfilename){
    HuffmanEncode huffen;
    huffen.encode(srcfilename.data(), destfilename.data());
}

void unzip(const std::string& srcfilename, const std::string& destfilename){
    HuffmanDecode huffde;
    huffde.decode(srcfilename.data(), destfilename.data());
}



// int main(int argc, char* argv[]){
    
//     std::string com = argv[1];
//     std::string src_1 = argv[2];
//     std::string des_1 = argv[3];
//     std::string des_2 = "a";

//     std::cout << com << std::endl;
//     std::cout << src_1 << std::endl;
//     std::cout << src_1 << std::endl;

//     if(argc  == 6){
//         des_2 = argv[5];
//         std::cout << des_2 << std::endl;
//     }
    

//     if(com == "zip"){
//         compression(src_1, des_1);
//     }
//     else if(com == "unzip"){
//         unzip(src_1, des_1);
//     }
//     else if(com == "myzip&&unzip"){
//         compression(src_1, des_1);
//         unzip(des_1, des_2);
//     }
//     else{
//         std::cout << "unknown command" <<std::endl;
//     }

//     std::cout << "按回车键退出...";
//     std::cin.get();  // 等待用户输入
    
//     return 0;

// }

#endif