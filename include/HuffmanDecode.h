#ifndef _HuffmanDecode_H
#define _HuffmanDecode_H

#include <map>
#include <fstream>

#include "HuffmanTree.h"

using unchar = unsigned char;

namespace huffman{
    class HuffmanDecode{
        private:
            fileHead *_fileHead;
            HuffmanTree *_htree;
            std::map<unchar, int> _cfMap;
            std::map<unchar, std::string> _codeMap;
            std::map<std::string, unchar> _decodeMap;

            long _getfilesize(const char* strfilename){
                std::ifstream in(strfilename);
                if(!in.is_open()) return 0;
                
                //直接指向文件末尾
                in.seekg(0, std::ios_base::end);
                std::streampos sp = in.tellg();
                in.close();
                // std::cout << "filesize:" << sp <<std::endl;
                return sp;
            }

            static bool _isleaf(Node* n){
                return n->left == nullptr && n->right == nullptr;
            }

            bool _getcharfreq(const char* filename){
                std::ifstream is(filename, std::ios::binary);
                if(!is){
                    printf("%s failed to read", filename);
                    return false;
                }

                is.read((char*)_fileHead, sizeof(fileHead));
                // std::cout << _fileHead->flag <<std::endl;
                // std::cout<< static_cast<int>(_fileHead->character_varity) <<std::endl;
                // std::cout<< static_cast<int>(_fileHead->lastValidBit) <<std::endl;
                if(!((_fileHead->flag[0] == 'n') && _fileHead->flag[1] == 'a'
                   && _fileHead->flag[2] == 'g' && _fileHead->flag[3] == 'i'
                   && _fileHead->flag[4] == 's' && _fileHead->flag[5] == 'a')){
                    // printf("not support file format: %s", filename);
                    std::cout << "[!] ERROR: Bad Compress header, please check your password.\n";
                    exit(-1);
                }

                //读取所有字符及其对应频率
                for (int i = 0; i < static_cast<int>(_fileHead->character_varity); ++i){
                    charFreq cf;
                    is.read((char*)&cf, sizeof(cf));
                    // std::cout << static_cast<int>(cf.character) << "    " << cf.freq << std::endl;
                    _cfMap.insert(std::pair<char, int>(cf.character, cf.freq));
                }
                is.close();
                return true;
            }

            bool _decode(const char* srcfilename, const char* destfilename){
                long filesize = _getfilesize(srcfilename);

                std::ifstream is(srcfilename, std::ios::binary);
                if(!is){
                    printf("%s failed to read", srcfilename);
                    return false;
                }
                is.seekg(sizeof(fileHead) + sizeof(charFreq) * _fileHead->character_varity);

                Node node = _htree->getHuffmanTree();
                Node* pNode = &node;

                std::ofstream io(destfilename, std::ios::binary);
                if(!io){
                    printf("creat file fialed: %s", destfilename);
                    return false;
                }

                unchar value;
                std::string code;
                int index = 0;
                long curlocation = is.tellg();
                // std::cout << "datapoint" << curlocation <<std::endl;
                is.read((char*)&value, sizeof(unchar));
                while(1){
                    if(_isleaf(pNode)){
                        unchar character = _decodeMap[code];
                        // std::cout << "index" << index <<"       " ;
                        // std::cout << character <<":" <<code << "        ";
                        io.write((char*)&character, sizeof(unchar));
                        if(curlocation >= filesize && index >= _fileHead->lastValidBit){
                            break;
                        }
                        code.clear();
                        pNode = &node;      //重新指向huffman的根节点
                    }

                    if(GET_BYTE(value, index)){
                        code += '1';
                        pNode = pNode->right;
                    }
                    else{
                        code += '0';
                        pNode = pNode->left;
                    }
                    if(++index >= 8){
                        index = 0;
                        // std::cout << "curlocation" <<curlocation <<std::endl;
                        is.read((char*)&value, sizeof(unchar));
                        curlocation = is.tellg();
                    }
                }
                is.close();
                io.close();
                return true;

            }
        public:
            bool decode(const char* srcfilename, const char* destfilename){
                if(!_getcharfreq(srcfilename))
                    return false;
                long filesize = _getfilesize(srcfilename);
                _htree = new HuffmanTree(_cfMap);
                //获取每个字符对应的哈弗曼编码
                _htree->HuffmanCode(_codeMap);

                for(auto it : _codeMap){
                    _decodeMap.insert(std::pair<std::string, unchar>(it.second, it.first));
                }

                return _decode(srcfilename, destfilename);
            }

            HuffmanDecode() : _fileHead(nullptr), _htree(nullptr){
                _fileHead = new fileHead();
            }
            ~HuffmanDecode(){
                if(!_fileHead) delete _fileHead;
                if(!_htree) delete _htree;
            }

    };

}




#endif