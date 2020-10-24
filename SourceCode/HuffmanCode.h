//Textcode.huf存放文本文件编码后结果，Translation.txt存放译码结果
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
typedef struct{
    unsigned int weight;
    unsigned int parent;
    unsigned int lchild;
    unsigned int rchild;
}Node,*HuffmanTree;
typedef char *HuffmanCode;

//开始菜单
void StartMenu();
//选择权值最小的两个结点
void Select(HuffmanTree *Leaf,int n,int *Leaf1,int *Leaf2);
//生成哈夫曼树
void GenerateHuffmanTree(HuffmanTree *huffTree,int w[],int n);
//生成哈夫曼编码
void Encode(HuffmanTree *huffTree,HuffmanCode *huffCode,int n,int ln[]);
//压缩写入.huf文件
void Compression(HuffmanTree *huffTree,HuffmanCode *huffCode,int n,char filename[20],char *position);
//解压缩（译码过程）
void TransCode(HuffmanTree *huffTree,char *position,int n,char filename[20]);
//计算压缩率
void CompressionRate(char filename[20]);
//可供选择的文件
void Choices();

