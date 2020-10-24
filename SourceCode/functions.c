#include"HuffmanCode.h"
//开始菜单
void StartMenu()
{
    printf("*********************************************************************\n");
    printf("*   欢迎使用本系统！本系统提供文本文件的哈夫曼编码/译码功能         *\n");
    printf("*                     功能列表如下：                                *\n");
    printf("*                 1.读取一个文本文件                                *\n");
    printf("*                 2.生成该文件的哈夫曼编码                          *\n");
    printf("*                 3.对该文本文件进行编码                            *\n");
    printf("*                 4.编码后文件与原文件大小对比，查看压缩率          *\n");
    printf("*                 5.根据编码进行反向译码                            *\n");
    printf("*                 0.退出本系统                                      *\n");
    printf("*********************************************************************\n");
}
//选择权值最小的两个结点
void Select(HuffmanTree *Leaf,int n,int *Leaf1,int *Leaf2)
{
    int i;
    int minLeaf;
    for(i=1;i<=n;i++){
        if((*Leaf)[i].parent==0)
        {
            minLeaf=i;
            break;
        }
    }
    for(i=1;i<=n;i++)
    {
        if((*Leaf)[i].parent==0)
            if((*Leaf)[i].weight<(*Leaf)[minLeaf].weight)
                minLeaf=i;
    }
    *Leaf1=minLeaf;
    for(i=1;i<=n;i++){
        if((*Leaf)[i].parent==0&&i!=(*Leaf1))
        {
            minLeaf=i;
            break;
        }
    }
    for(i=1;i<=n;i++){
        if((*Leaf)[i].parent==0&&i!=(*Leaf1))
            if((*Leaf)[i].weight<(*Leaf)[minLeaf].weight)
                minLeaf=i;
    }
    *Leaf2=minLeaf;
}
//生成哈夫曼树
void GenerateHuffmanTree(HuffmanTree *huffTree,int w[],int n)
{
    int sumLeaf=2*n-1;
    int leaf1,leaf2;
    int i;
    *huffTree=(HuffmanTree)malloc((sumLeaf+1)*sizeof(Node));
    for(i=1;i<=n;i++)
    {
        (*huffTree)[i].weight=w[i-1];
        (*huffTree)[i].parent=0;
        (*huffTree)[i].lchild=0;
        (*huffTree)[i].rchild=0;
    }
    for(i=n+1;i<=sumLeaf;i++)
    {
        (*huffTree)[i].weight=0;
        (*huffTree)[i].parent=0;
        (*huffTree)[i].lchild=0;
        (*huffTree)[i].rchild=0;
    }
    printf("\n建造哈夫曼树过程如下：\n");
    for(i=n+1;i<=sumLeaf;i++)
    {
        Select(huffTree,i-1,&leaf1,&leaf2);
        (*huffTree)[leaf1].parent=i;
        (*huffTree)[leaf2].parent=i;
        (*huffTree)[i].lchild=leaf1;
        (*huffTree)[i].rchild=leaf2;
        (*huffTree)[i].weight=(*huffTree)[leaf1].weight+(*huffTree)[leaf2].weight;
        printf("被合并的两结点权值：%d %d，合并成的结点权值：%d\n",(*huffTree)[leaf1].weight,(*huffTree)[leaf2].weight,(*huffTree)[i].weight);
    }
    printf("\n哈夫曼树建造完毕。\n");
}
//生成哈夫曼编码
void Encode(HuffmanTree *huffTree,HuffmanCode *huffCode,int n,int ln[])
{
   // FILE *fw,*fp;//编码并写入文件
   // char ch;
    int i,k,start,p;
    unsigned int mark;
    char *cd=(char*)malloc(n*sizeof(char));
    huffCode=(HuffmanCode*)malloc((n+1)*sizeof(char*));
    cd[n-1]='\0';
    for(i=1;i<=n;i++)
    {
        start=n-1;
        for(mark=i,p=(*huffTree)[i].parent;p!=0;mark=p,p=(*huffTree)[p].parent)
        {
            if((*huffTree)[p].lchild==mark)
                cd[--start]='0';
            else cd[--start]='1';
        }
        huffCode[i]=(char*)malloc((n-start)*sizeof(char));
        strcpy(huffCode[i],&cd[start]);
    }
    free(cd);
    printf("编码结果如下：\n");
	for(k=0,i=1;k<93;k++)
	{
		if(ln[k]!=0)
		{
            if(k==91) printf("换行符 : %s\n",huffCode[i]);
            else if(k==92) printf("回车符 : %s\n",huffCode[i]);
            else printf("%c : %s\n",k+32,huffCode[i]);
			i++;
		}
	}
}
//压缩写入.huf文件
void Compression(HuffmanTree *huffTree,HuffmanCode *huffCode,int n,char filename[20],char *position)
{
    unsigned char char_temp;//暂存8bits字符
    unsigned long file_len=0;//统计文件总长度，初始值设为0
    char code_buf[25]="\0";//编码缓冲区
    unsigned int code_len;//编码长度
    FILE *fp,*fw;
    //非全局变量，每个函数运行时均需要重新进行编码
    int i,start,p,pos;
    unsigned int mark;
    char *cd=(char*)malloc(n*sizeof(char));
    huffCode=(HuffmanCode*)malloc((n+1)*sizeof(char*));
    cd[n-1]='\0';
    for(i=1;i<=n;i++)
    {
        start=n-1;
        for(mark=i,p=(*huffTree)[i].parent;p!=0;mark=p,p=(*huffTree)[p].parent)
        {
            if((*huffTree)[p].lchild==mark)
                cd[--start]='0';
            else cd[--start]='1';
        }
        huffCode[i]=(char*)malloc((n-start)*sizeof(char));
        strcpy(huffCode[i],&cd[start]);
    }
    free(cd);//编码完成
    //开始写入huf文件
    fp=fopen(filename,"rb");
    fw=fopen("Textcode.huf","wb");
    fread((char*)&char_temp,sizeof(unsigned char),1,fp);//二进制形式读入一个字符，每次8bits
    while(!feof(fp))
    {
        for(i=1;i<=n;i++)//循环找到匹配字符来进行编码
            if(char_temp==position[i-1])
            {
                pos=i-1;
                strcat(code_buf,huffCode[pos+1]);//将该字符对应哈夫曼编码暂存入缓存区以待处理
            }
        while(strlen(code_buf)>=8)//以8位(一个字节长度)为处理单元
        {
            char_temp='\0';//清空字符暂存空间，改为暂存字符对应编码
            for(i=0;i<8;++i)
            {
                char_temp<<=1;//左移一位，为下一个bit腾出位置
                if(code_buf[i]=='1')
                    char_temp|=1;//当编码为1，使用或操作符将其添加到字节最低位
            }
            fwrite((char*)&char_temp,sizeof(unsigned char),1,fw);//将字节对应编码存入文件
            strcpy(code_buf,code_buf+8);//编码缓存去除已处理的前8位
        }
        fread((char*)&char_temp,sizeof(unsigned char),1,fp);//每次读取8bits
    }
    code_len=strlen(code_buf);//处理最后不足8bits的编码
    if(code_len>0)
    {
        char_temp='\0';
        for(i=0;i<code_len;++i)
        {
            char_temp <<= 1;
            if(code_buf[i]=='1')
                char_temp |= 1;
        }
        char_temp <<= 8-code_len;//将编码字段从尾部移到字节高位，余部补0
        fwrite((char*)&char_temp,sizeof(unsigned char),1,fw);//存入最后一个字节
    }
    fclose(fp);
    fclose(fw);
}   
//解压缩（译码过程）
void TransCode(HuffmanTree *huffTree,char *position,int n,char filename[20])
{
    int i,root=2*n-1;//总结点个数
    unsigned long file_len;//文本总长度
    unsigned long writen_len=0;//控制文件写入长度
    unsigned char code_temp;//暂存8bits编码
    //打开读取与写入的文件
    FILE *fp,*fw;
	FILE *fl;
	fl=fopen(filename,"rb");
	fseek(fl,0L,SEEK_END);
	file_len=ftell(fl);
	fclose(fl);
    fp=fopen("Textcode.huf","rb");
    fw=fopen("Translation.txt","wb");

    while(1)
    {
        fread((char*)&code_temp,sizeof(unsigned char),1,fp);//读8bits
        for(i=0;i<8;++i)
        {
            //由根向下直至叶结点正向匹配编码对应字符
            if(code_temp & 128)//若为0
                root=(*huffTree)[root].rchild;
            else
                root=(*huffTree)[root].lchild;
            
            if((*huffTree)[root].lchild==0&&(*huffTree)[root].rchild==0)
            {
                fwrite((char*)&position[root-1],sizeof(unsigned char),1,fw);
                writen_len++;
                if(writen_len==file_len)//控制长度跳出for循环
                    break;
                root=2*n-1;//root复位
            }
            code_temp<<=1;//左移供匹配
        }
        if(writen_len==file_len) break;
    }
    //关闭文件
    fclose(fp);
    fclose(fw);
}
//计算压缩率
void CompressionRate(char filename[20])
{
    FILE *fp1,*fp2;
    double len1,len2;
    fp1=fopen("Textcode.huf","rb");
    fp2=fopen(filename,"rb");;
    fseek(fp1,0L,SEEK_END);
    fseek(fp2,0L,SEEK_END);
    len1=ftell(fp1);
    len2=ftell(fp2);
    printf("\n该文本文档压缩率为：百分之%2.3lf\n",100-len1/len2*100);
    fclose(fp1);
    fclose(fp2);
}
void Choices()
{
	printf("目前可供选择的文件有： 1.BestSeller.txt  2.Gatsby.txt 3.Rye.txt\n");
}