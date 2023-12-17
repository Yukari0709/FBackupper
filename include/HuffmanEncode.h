#ifndef _HuffmanEncode_H
#define _HuffmanEncode_H

#include <map>
#include <fstream>

#include "HuffmanTree.h"

using unchar = unsigned char;

namespace huffman{
    class HuffmanEncode{
        private:
            std::map<unchar, int> _cfMap;
            std::map<unchar, std::string> _codeMap;
            
            //逐字节读入原文件，统计各字符出现频率
            bool _getcharfreq(const char* filename){
                unchar ch;
                std::ifstream is (filename, std::ios::binary);
                if(!is.is_open()){
                    printf("%s failed to read!", filename);
                    return false;
                }
                is.read((char*)&ch, sizeof(unchar));
                while(!is.eof()){
                    _cfMap[ch]++;
                    is.read((char*)&ch, sizeof(unchar));
                }
                is.close();
                return true;
            }

            //计算压缩后所有哈弗曼编码的位数并截取后3位
            int _getlastvalidbit(){
                int sum = 0;
                for (auto it : _codeMap){
                    sum += it.second.size() * _cfMap.at(it.first);
                    sum &= 0xFF;
                }
                sum &= 0x7;
                // std::cout << "sum :" <<sum <<std::endl;
                return sum == 0 ? 8 : sum;
            }

            bool _encode(const char* srcfilename, const char* destfilename){
                unchar ch;
                unchar value;
                int bitindex = 0;
                //设置文件头信息
                fileHead filehead = {'n', 'a', 'g', 'i', 's', 'a'};
                filehead.character_varity = (unchar) _cfMap.size();
                filehead.lastValidBit = _getlastvalidbit();
                // std::cout <<"_getlatsvalidbit" << _getlastvalidbit() << "   " <<"filehead.last" <<static_cast<int>(filehead.lastValidBit) <<std::endl;

                std::ifstream is(srcfilename, std::ios::binary);
                if(!is.is_open()){
                    printf("%s failed to open", srcfilename);
                    return false;
                }
                std::ofstream io(destfilename, std::ios::binary);
                if(!io.is_open()){
                    printf("%s failed to read", destfilename);
                    return false;
                }

                //写入头文件，以及所有cfMap
                io.write((char*)&filehead, sizeof(filehead));
                // std::cout << filehead.flag <<std::endl;
                // std::cout<< static_cast<int>(filehead.character_varity) <<std::endl;
                // std::cout<< static_cast<int>(filehead.lastValidBit) <<std::endl;
                for (auto i : _cfMap){
                    charFreq cf(i);
                    // std::cout << static_cast<int>(cf.character) << "    " << cf.freq << std::endl;
                    io.write((char*)&cf, sizeof(charFreq));
                }
                // std::cout << "datapoint:" << io.tellp() <<std::endl;

                is.read((char*)&ch, sizeof(unchar));
                while(!is.eof()){
                    std::string code = _codeMap.at(ch);
                    // std::cout << ch << ":" << code << "     ";
                    for(auto c : code){
                        if(c == '0'){
                            CLR_BYTE(value, bitindex);
                        }
                        else{
                            SET_BYTE(value, bitindex);
                        }
                        ++bitindex;
                        if(bitindex >= 8){
                            bitindex = 0;
                            // std::cout << "write_value" << static_cast<int>(value) << std::endl;
                            io.write((char*)&value, sizeof(unchar));
                        }
                    }
                    is.read((char*)&ch, sizeof(unchar));
                }

                if(bitindex){
                    io.write((char*)&value, sizeof(unchar));
                }
                std::streampos sp = io.tellp();
                // std::cout << "filesize:" << sp <<std::endl;
                is.close();
                io.close();
                return true;

            }
        
        public:
            bool encode(const char* srcfilename, const char* destfilename){
                if(!_getcharfreq(srcfilename))
                    return false;
                HuffmanTree ht(_cfMap);
                ht.HuffmanCode(_codeMap);
                return _encode(srcfilename, destfilename);
            }
    };

}



#endif