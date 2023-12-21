#include "HuffmanTree.h"
#include "HuffmanDecode.h"
#include "HuffmanEncode.h"
#include "ZIP_UNZIP.h"

void huffman::compression(const std::string& srcfilename, const std::string& destfilename){
    HuffmanEncode huffen;
    huffen.encode(srcfilename.data(), destfilename.data());
}

void huffman::unzip(const std::string& srcfilename, const std::string& destfilename){
    HuffmanDecode huffde;
    huffde.decode(srcfilename.data(), destfilename.data());
}
