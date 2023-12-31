#ifndef _AES_H
#define _AES_H

#include <string>


#define RELESE(P) if(P) {delete P;  P = NULL;}
#define RELESE_ARRAY(P) if(P) {delete[] P;  P = NULL;}

namespace En_Decryption{
    int AesEncryptFile(std::string src_path, std::string dest_path, const  char Key[32] = "default_keys");
    int AesDecryptFile(std::string src_path, std::string dest_path, const  char Key[32] = "default_keys");
}

#endif