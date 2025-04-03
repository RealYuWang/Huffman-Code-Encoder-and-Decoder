#include"HuffmanCode.h"
int main()
{
	HuffmanTree huffTree;
	HuffmanCode huffCode;
	FILE *fp;
	char ch,filename[20];
    int i=0,k=0,ln[93];//存放各个字符出现次数，93为文本文档可能出现的所有字符
    int *w,validNum=0;//w存放各字符权值，validNum为字符种类数量
    char *position;//编码需要,记录对应位置的字符
    int control,flag=1;

    for(i=0;i<93;i++) ln[i]=0;//各字符出现次数初始值为0
    while(1)
    {
        StartMenu();
        printf("请输入你要进行的操作（数字）：");
        scanf("%d",&control);
        switch(control){
            case 1:
			Choices();
            printf("输入你想读取的文件名："); 
            scanf("%s",filename);
            if(!(fp=fopen(filename,"r")))
            {
                printf("无法打开文件！请确保输入已存在的文件名！\n");
                exit(-2);
            }
            //此循环统计不同字符出现的次数
            while(ch!=EOF)
            {
                ch=fgetc(fp);
                if(ch>=32&&ch<=122)
                    ln[ch-' ']++;//这91个均为常规字符，包括空格
                else if(ch==10)
                    ln[91]++;//此为换行符
                else 
                    ln[92]++;
                putchar(ch);//输出文档内容
            }
            fclose(fp);
			printf("\n");
            //输出统计结果
            for(i=0;i<93;i++)
            {
                if(ln[i]!=0){
                    validNum++;//计算出现的字符的种类
                    if(i==91) printf("回车符 出现次数 %d\n",ln[i]);
                    else if(i==92) printf("换行符 出现次数 %d\n",ln[i]);
                    else printf(" %c 出现次数：%d\n",' '+i,ln[i]);
                }
            }
	        printf("\n本文中共有%d种不同的字符。\n",validNum);
            //因为起初不知道具体字府种类，因此现在动态分布存放权重的数组
            w=(int*)malloc((validNum)*sizeof(int));
            position=(char*)malloc((validNum)*sizeof(char));
            //输入数据
	        for(i=0;i<93;i++)
                if(ln[i]!=0){
                    w[k]=ln[i];
                    if(i==91) position[k]=10;
                    else if(i==92) position[k]=13;
                    else position[k]=i+' ';
                    k++;
                }
            printf("%s文件打开并读取成功。\n",filename);
            printf("请执行下一步操作。\n\n\n");
            break;
            case 2:
            GenerateHuffmanTree(&huffTree,w,validNum);
	        Encode(&huffTree,&huffCode,validNum,ln);
            printf("请执行下一步操作。\n\n\n");
            break;
            case 3:
            Compression(&huffTree,&huffCode,validNum,filename,position);
            printf("编码完成。已写入Textcode.huf文件中。\n");
            printf("请执行下一步操作。\n\n\n");
            break;
            case 4:
            CompressionRate(filename);
            printf("请执行下一步操作。\n\n\n");
            break;
            case 5:
            TransCode(&huffTree,position,validNum,filename);
            printf("翻译完成。已写入Translation.txt文件中\n");
            printf("请执行下一步操作。\n\n\n");
            break;
            case 0:
            flag=0;
            break;
            default:printf("无效指令！请重新输入！\n");
            continue;
        }
        if(flag==0)
        break;
    }
    printf("\n\n感谢您的使用！您现在可以在程序所在文件中找到Textcode.huf与Translation.txt文件。\n\n\n");
    return 0;
}
