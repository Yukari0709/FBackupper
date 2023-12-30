#include <openssl/aes.h>
#include <cstring>
#include <fstream>
#include <iostream>

#include "aes.h"

using unchar  = unsigned char;

int En_Decryption::AesEncryptFile(std::string src_path, std::string dest_path, const char key[32] = "default_keys"){
    //AES块大小通常是128bits，即16字节
    int encrypt_buffer_size = 16;

    std::ifstream is(src_path.c_str(), std::ios::binary);
    std::ofstream io(dest_path.c_str(), std::ios::binary);

    if(!is){
        std::printf("Can't open src file : %s", src_path.c_str());
        return 1;
    }
    if(!io){
        std::printf("Can't open dest file : %s", dest_path.c_str());
        return 1;
    }

    //char->unchar，保证其占32字节
    unchar aes_keybuf[32];
    std::memset(aes_keybuf, 0 , sizeof(aes_keybuf));
    std::strcpy((char *)aes_keybuf, key);
    AES_KEY aes_key;
    //设置加密密匙，32字节对应256bits
    AES_set_encrypt_key(aes_keybuf, 256, &aes_key);

    char *in_data = new char[encrypt_buffer_size + 1];
    char *out_data = new char[encrypt_buffer_size + 1];
    
    while(!is.eof()){
        is.read(in_data, encrypt_buffer_size);

        if(is.gcount() < encrypt_buffer_size){
            io.write(in_data, is.gcount());
        }
        else{
            AES_encrypt((const unchar *)in_data, (unchar *)out_data, &aes_key);
            io.write(out_data, is.gcount());
        }
    };

    is.close();
    io.close();

    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);
    
    return 0;
}

int En_Decryption::AesDecryptFile(std::string src_path, std::string dest_path, const char key[32] = "default_keys"){
    int encrypt_buffer_size = 16;

    std::ifstream is(src_path.c_str(), std::ios::binary);
    std::ofstream io(dest_path, std::ios::binary);

    if(!is){
        std::printf("Can't open src file : %s", src_path.c_str());
        return 1;
    }
    if(!io){
        std::printf("Can't open dest file : %s", dest_path.c_str());
        return 1;
    }

    unchar aes_keybuf[32];
    std::memset(aes_keybuf, 0, sizeof(aes_keybuf));
    std::strcpy((char *)aes_keybuf, key);

    AES_KEY aes_key;
    //设置解密密匙
    AES_set_decrypt_key(aes_keybuf, 256, &aes_key);

    char *in_data = new char[encrypt_buffer_size + 1];
    char *out_data = new char[encrypt_buffer_size + 1];

    while(!is.eof()){
        is.read(in_data, encrypt_buffer_size);
        if(is.gcount() < encrypt_buffer_size){
            io.write(in_data, is.gcount());
        }
        else{
            AES_decrypt((unchar *)in_data, (unchar *)out_data, &aes_key);
            io.write(out_data, is.gcount());
        }
    };

    io.close();
    is.close();

    RELESE_ARRAY(in_data);
    RELESE_ARRAY(out_data);

    return 0;
}


// int main(){
//     std::string src_path = "./your.jpeg";
//     std::string encrypy_path = src_path + ".enc";
//     std::string dest_path = "./your_new.jpeg";

//     std::cout << "Source path: " << src_path << std::endl;
//     std::cout << "Encrypted path: " << encrypy_path << std::endl;
//     std::cout << "Decrypted path: " << dest_path << std::endl;

//     En_Decryption::AesEncryptFile(src_path, encrypy_path);
//     std::cout << "Encryption completed." << std::endl;
    
//     En_Decryption::AesDecryptFile(encrypy_path, dest_path);
//     std::cout << "Decryption completed." << std::endl;

//     return 0;


// }

