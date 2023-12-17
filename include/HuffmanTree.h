#ifndef _HuffmanTree_H
#define _HuffmanTree_H
#include <utility>
#include <iostream>
#include <queue>
#include <map>
#include <string>

//定义宏运算,写入是从前往后，读取应为从后往前
//查看Byte对应的index值是否为1
#define GET_BYTE(Byte, index) (((Byte) & (1 << ((index) ^ 7))) != 0)
//index位置1,set_byte
#define SET_BYTE(Byte, index) ((Byte) |= (1 << ((index) ^ 7)))
//index位置0,clear_byte
#define CLR_BYTE(Byte, index) ((Byte) &= (~(1 << ((index) ^ 7))))

using unchar = unsigned char;

//文件头共占用八个字节
struct fileHead{
    char flag[6] ;//压缩后文件头标志，nagisa
    unchar character_varity;    //字符种类
    unchar lastValidBit;        //最后一个字节的有效位数
    char unused[8];			//待用空间
};

struct charFreq{
    unchar character;
    int freq;
    charFreq() {};
    charFreq(const std::pair<char, int>& x)
        :character(x.first), freq(x.second) {}
};


namespace huffman{
    struct Node{
        unchar c;
        int freq;
        Node *left;
        Node *right;
        Node(unchar _c, int f, Node *l = nullptr, Node *r = nullptr)
            : c(_c), freq(f), left(l), right(r) {}
        //重载<，优先队列std::heap是最小堆
        bool operator<(const Node &node) const{
            return freq > node.freq;
        }
    };

    class HuffmanTree{
        private:
            std::priority_queue<Node> q;

            void _deleteTree(Node* n){
                if(!n) return;
                _deleteTree(n->left);
                _deleteTree(n->right);
                delete n;
            }

            static bool _isLeaf(Node* n){
                return n->left == nullptr && n->right == nullptr;
            }

            //深度优先遍历算法，遍历完整个哈夫曼树并对每个叶子结点生成一个codeMap
            void _HuffmanCode(Node *root, std::string& prefix, std::map<unchar, std::string>& codeMap){
                std::string tmp = prefix;
                if(root->left != nullptr){
                    prefix += '0';
                    if(_isLeaf(root->left)){
                        codeMap[root->left->c] = prefix;
                    }
                    else{
                        _HuffmanCode(root->left, prefix, codeMap);
                    }
                }
                if(root->right != nullptr){
                    prefix = tmp;
                    prefix += '1';
                    if(_isLeaf(root->right)){
                        codeMap[root->right->c] = prefix;
                    }
                    else{
                        _HuffmanCode(root->right, prefix, codeMap);
                    }
                }
            }

        public:
            HuffmanTree(const std::map<unchar, int>& afMap){
                for(auto i : afMap){
                    Node n(i.first, i.second);
                    q.push(n);
                }
                makeHuffmanTree();
            }

            ~HuffmanTree(){
                Node node = q.top();
                _deleteTree(node.left);
                _deleteTree(node.right);
            }

            void makeHuffmanTree(){
                while(q.size() != 1){
                    //从优先队列中将两个最小的节点pop出来，分别为新节点left和right
                    Node *left = new Node(q.top());
                    q.pop();
                    Node *right = new Node(q.top());
                    q.pop();
                    Node node('R', left->freq + right->freq, left, right);
                    //将合并后的节点重新push到优先队列的最小堆中
                    q.push(node);
                }
            }

            void HuffmanCode(std::map<unchar, std::string>& codeMap){
                Node node = q.top();
                std::string prefix;
                _HuffmanCode(&node, prefix, codeMap);
            }

            Node getHuffmanTree(){
                return q.top();
            }
    };

}



#endif