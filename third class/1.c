#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAXCODE 100 // 最多字符个数
#define MAXTEXT 100 // 最大明文长度

// 哈夫曼树节点
typedef struct HuffmanNode{
    int weight; // 权值
    char ch; // 叶子节点字符
    int parent; // 父亲节点
    int lchild; // 左孩子节点
    int rchild; // 右孩子节点
}HtNode;

// 哈夫曼树
typedef struct HuffmanTree{
    HtNode ht[2*MAXCODE]; // 哈夫曼树节点（有效索引从1开始）
    int root; // 哈夫曼树根在数组中的索引
}HtTree,*PHtTree;

// 哈夫曼编码
typedef char **HuffmanCode;

PHtTree createHtTree(int n,int weight[],char huff[]); // 创建哈夫曼树
void Select(PHtTree pht,int pos,int *x1,int *x2); // 寻找当前无父亲节点的权值最小的两个节点
HuffmanCode codeHuffman(PHtTree pht,int n); // 哈夫曼编码
char* translate(PHtTree pht,int n,char text[],HuffmanCode code); // 明文转译为密文并打印
int getIndex(PHtTree pht,int n,char c); // 获取节点索引
void retranslate(PHtTree pht,char ciphertext[]); // 译码
void freeAll(PHtTree pht,int n,HuffmanCode code,char ciphertext[]); // 释放指针内存

int main()
{
    int n; // 字符集大小
    char huff[MAXCODE]; // 字符集
    int weight[MAXCODE]; // 权值
    char text[MAXTEXT]; // 明文
    PHtTree tree; // 哈夫曼树
    HuffmanCode code; // 哈夫曼编码
    char *ciphertext; // 密文

    // 读取字符集大小
    scanf("%d",&n);
    if(n>MAXCODE || n<2){ // 检查n范围
        printf("Please enter n not more than %d.\n",MAXCODE);
        exit(EXIT_FAILURE);
    }
    // 读取字符集
    for(int i=0;i<n;i++)
        scanf(" %c",&huff[i]); // 跳过空格
    // 读取权值
    for(int i=0;i<n;i++)
        scanf("%d",&weight[i]);
    getchar(); // 去除换行符
    // 读取明文
    scanf("%s",text);
    if(strlen(text)>MAXTEXT || strlen(text)==0){ // 检查明文长度
        printf("Please enter text whose length is not more than %d.\n",MAXTEXT);
        exit(EXIT_FAILURE);
    }

    // 创建哈夫曼树
    tree=createHtTree(n,weight,huff);

    // 获取哈夫曼编码
    code=codeHuffman(tree,n);
    
    // 转译密文及打印
    ciphertext=translate(tree,n,text,code);

    // 译码
    retranslate(tree,ciphertext);

    // 释放内存
    freeAll(tree,n,code,ciphertext);

    return 0;
}

// 创建哈夫曼树
PHtTree createHtTree(int n,int weight[],char huff[])
{
    PHtTree pht; // 哈夫曼树声明
    pht=(PHtTree)malloc(sizeof(HtTree));
    if(!pht){ // 如果未能分配内存
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    pht->root=0; // 初始化根索引
    // 初始化节点
    for(int i=1;i<=2*n-1;i++){
        pht->ht[i].parent=0;
        pht->ht[i].lchild=0;
        pht->ht[i].rchild=0;
        if(i<=n){ // 叶子节点赋权值、对应字符
            pht->ht[i].weight=weight[i-1];
            pht->ht[i].ch=huff[i-1];
        }else{ // 其它节点置为0
            pht->ht[i].weight=0;
            pht->ht[i].ch='\0';
        }
    }

    // 循环构造内部节点
    for(int i=1;i<n;i++){
        int x1=0,x2=0; // 初始化左、右孩子索引
        Select(pht,n+i,&x1,&x2); // 求左右孩子的索引（x1<x2）
        // 更新左右孩子的父亲节点
        pht->ht[x1].parent=n+i;
        pht->ht[x2].parent=n+i;
        // 新的内部节点的权值为左右孩子权值的和
        pht->ht[n+i].weight=pht->ht[x1].weight+pht->ht[x2].weight;
        // 更新新节点的左右孩子
        pht->ht[n+i].lchild=x1;
        pht->ht[n+i].rchild=x2;
        pht->root=n+i;
    }

    return pht; // 返回哈夫曼树指针
}

// 寻找当前无父亲节点的权值最小的两个节点
void Select(PHtTree pht,int pos,int *x1,int *x2)
{
    int m1=INT_MAX,m2=INT_MAX; // 初始化赋阈值为最大可能值
    for(int i=1;i<pos;i++){
        if(pht->ht[i].weight<m1 && pht->ht[i].parent==0){
            m2=m1;
            *x2=*x1;
            m1=pht->ht[i].weight;
            *x1=i; // 最小权值节点的索引
        }else if(pht->ht[i].weight<m2 && pht->ht[i].parent==0){
            m2=pht->ht[i].weight;
            *x2=i; // 次小权值节点的索引
        }
    }
}

// 哈夫曼编码
HuffmanCode codeHuffman(PHtTree pht,int n)
{
    // 哈夫曼编码字符串数组（有效索引从1开始）
    HuffmanCode HC=(HuffmanCode)malloc((n+1)*sizeof(char*));
    if(!HC){ // 如果未能分配内存
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    char *cd=(char*)malloc(n*sizeof(char)); // 临时字符串
    if(!cd){ // 如果未能分配内存
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    cd[n-1]='\0';
   
    for(int i=1;i<=n;i++){
        int start; // 字符串分配字符的起始位置（从后往前分配）
        int current; // 当前节点索引
        int father; // 当前节点的父亲节点索引
        start=n-1; // 从cd[n-2]开始赋值
        current=i; // 当前叶子节点
        father=pht->ht[i].parent; // 叶子节点的父亲节点

        while(father!=0){ // 父亲节点非0（未到达哈夫曼树根）
            if(pht->ht[father].lchild==current) // 当前节点是父亲的左孩子
                cd[--start]='0';
            else // 当前节点是父亲的右孩子
                cd[--start]='1';
            current=father; // 更新当前节点
            father=pht->ht[father].parent; // 更新父亲节点 
        }

        HC[i]=(char*)malloc((n-start+1)*sizeof(char)); // +1为补充'\0'
        if(!HC[i]){ // 如果未能分配内存
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        strcpy(HC[i],cd+start); // 将有效字符串复制至哈夫曼编码字符串
    }
    free(cd); // 是否临时字符串

    return HC; // 返回哈夫曼编码
}

// 明文转译为密文并打印
char* translate(PHtTree pht,int n,char text[],HuffmanCode code)
{
    // 密文
    char *ciphertext=(char*)malloc(strlen(text)*MAXCODE*sizeof(char));
    if(!ciphertext){ // 如果未能分配内存
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    ciphertext[0]='\0'; // 初始化密文

    for(int i=0;i<strlen(text);i++){
        int idx=getIndex(pht,n,text[i]); // 获取当前明文字符的索引
        if(!idx){ // idx为0，此时明文存在无效字符
            printf("Character %c doesn't exit.\n",text[i]);
            exit(EXIT_FAILURE);
        }
        strcat(ciphertext,code[idx]); // 将当前字符编码添加至密文字符串末尾
    }
    printf("%s",ciphertext); // 打印密文字符串
    printf("\n");
    return ciphertext; // 返回密文字符串
}

// 获取节点索引
int getIndex(PHtTree pht,int n,char c)
{
    for(int i=1;i<=n;i++){
        if(pht->ht[i].ch==c) return i;
    }
    return 0; // 未获取索引（该字符c不在字符集中）
}

// 译码
void retranslate(PHtTree pht,char ciphertext[])
{
    int root=pht->root; // 获取树的根节点
    int current=root; // 当前节点初始为根节点
    for(int i=0;i<strlen(ciphertext);i++){
        if(ciphertext[i]=='0') // 进入左子树
            current=pht->ht[current].lchild;
        if(ciphertext[i]=='1') // 进入右子树
            current=pht->ht[current].rchild;
        
        // 若current为叶子节点（即没有左右孩子结点）
        if(pht->ht[current].lchild==0 && pht->ht[current].rchild==0){
            // 输出该叶子节点对应的字符
            printf("%c",pht->ht[current].ch);
            // 译码完一个字符，重置为根节点继续译码
            current=root;
        }
    }
    printf("\n");
}

// 释放指针内存
void freeAll(PHtTree pht,int n,HuffmanCode code,char ciphertext[])
{
    free(pht); // 释放哈夫曼树
    for(int i=1;i<=n;i++)
        free(code[i]);
    free(code); // 释放哈夫曼编码
    free(ciphertext); // 释放密文字符串
}