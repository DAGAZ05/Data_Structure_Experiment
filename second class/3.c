/*稀疏矩阵加法（十字链表法）*/

#include <stdio.h>
#include <stdlib.h>

// 十字链表节点
typedef struct OLNode{
    int i,j; // 行数、列数
    int element; // 元素
    struct OLNode *right,*down; // 右指针、下指针
}OLNode,*OLink;

// 十字链表
typedef struct crossList{
    OLink *rhead,*chead; // 行头节点、列头节点数组指针
    int mu,nu,tu; // 行数、列数、非零元个数
}crossList,*PCrossList;

PCrossList createCrossList(int m,int n,int t); // 创建十字链表
PCrossList addCrossList(PCrossList M1,PCrossList M2); // 十字链表稀疏矩阵加法
void printCrossList(PCrossList S); // 打印十字链表稀疏矩阵
void freeCrossList(PCrossList M); // 释放链表内存

int main()
{
    PCrossList M1,M2,S; // 声明指向稀疏矩阵M1、M2、和矩阵S的十字链表指针
    int m,n,t1,t2; // 稀疏矩阵行数、列数、M1非零元个数、M2非零元个数声明
    
    scanf("%d %d %d %d",&m,&n,&t1,&t2); // 读取行数、列数、非零元个数
    getchar(); // 去除换行符

    // 读取两个稀疏矩阵
    M1=createCrossList(m,n,t1);
    M2=createCrossList(m,n,t2);

    // 稀疏矩阵相加并赋值给S
    S=addCrossList(M1,M2);
    
    // 打印和矩阵
    printCrossList(S);

    // 释放内存
    freeCrossList(M1);
    freeCrossList(M2);
    freeCrossList(S);

    return 0;
}

// 创建十字链表
PCrossList createCrossList(int m,int n,int t)
{
    PCrossList M=(PCrossList)malloc(sizeof(crossList)); // 分配链表内存
    if(!M){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    // 赋值M的行数、列数、非零元个数
    M->mu=m;
    M->nu=n;
    M->tu=t;

    M->rhead=(OLink*)malloc(m*sizeof(OLink)); // 分配行指针数组内存
    if(!(M->rhead)){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<m;i++){
        M->rhead[i]=NULL; // 初始每个行指针赋空
    }

    M->chead=(OLink*)malloc(n*sizeof(OLink)); // 分配列指针数组内存
    if(!(M->chead)){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    for(int j=0;j<n;j++){
        M->chead[j]=NULL; // 初始每个列指针赋空
    }

    for(int s=0;s<t;s++){
        OLink p=(OLink)malloc(sizeof(OLNode));
        if(!p){ // 内存分配失败
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }

        scanf("%d %d %d",&(p->i),&(p->j),&(p->element));
        p->i--;
        p->j--;
        getchar();
        
        // 行插入
        if(M->rhead[p->i]==NULL || M->rhead[p->i]->j>p->j){
            // 插入链表头部
            p->right=M->rhead[p->i];
            M->rhead[p->i]=p;
        }else{
            // 遍历插入
            OLink q;
            for(q=M->rhead[p->i];(q->right) && q->right->j<p->j;q=q->right);
            p->right=q->right;
            q->right=p;          
        }

        // 列插入
        if(M->chead[p->j]==NULL || M->chead[p->j]->i>p->i){
            // 插入链表头部
            p->down=M->chead[p->j];
            M->chead[p->j]=p;
        }else{
            // 遍历插入
            OLink q;
            for(q=M->chead[p->j];(q->down) && q->down->i<p->i;q=q->down);
            p->down=q->down;
            q->down=p;
        }
    }
    return M;
}

// 十字链表稀疏矩阵加法
PCrossList addCrossList(PCrossList M1,PCrossList M2)
{
    PCrossList S=(PCrossList)malloc(sizeof(crossList)); // 分配链表内存
    if(!S){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    // 赋值行数、列数、非零元个数
    S->mu=M1->mu;
    S->nu=M1->nu;
    S->tu=0;

    S->rhead=(OLink*)malloc(S->mu*sizeof(OLink)); // 分配行指针数组内存
    S->chead=(OLink*)malloc(S->nu*sizeof(OLink)); // 分配列指针数组内存
    if(!(S->rhead) || !(S->chead)){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<S->mu;i++){
        S->rhead[i]=NULL; // 初始每个行指针赋空
    }
    for(int i=0;i<S->nu;i++){
        S->chead[i]=NULL; // 初始每个列指针赋空
    }

    // 逐行归并
    for(int i=0;i<S->mu;i++){
        OLink p=M1->rhead[i]; // M1行指针
        OLink q=M2->rhead[i]; // M2行指针
        OLink tail=NULL; // 和矩阵S第i行链表的尾指针

        while(p!=NULL || q!=NULL){
            int col; // 临时列变量
            int sum; // 临时元素和变量
            if(p!=NULL && (q==NULL || p->j<q->j)){
                // M1中节点非空，M2中无节点或在M1后
                col=p->j;
                sum=p->element;
                p=p->right; // M1指针右移
            }else if(q!=NULL && (p==NULL || p->j>q->j)){
                // M2中节点非空，M1中无节点或在M2后
                col=q->j;
                sum=q->element;
                q=q->right; // M2指针右移
            }else{
                // 两节点位置相同
                col=p->j;
                sum=p->element+q->element; // 求和
                p=p->right; // M1指针右移
                q=q->right; // M2指针右移
            }

            // 和非0时赋值
            if(sum!=0){
                OLink node=(OLink)malloc(sizeof(OLNode));
                if(!node){ // 内存分配失败
                    printf("Memory allocation failed!\n");
                    exit(EXIT_FAILURE);
                }

                // 赋值操作
                node->i=i;
                node->j=col;
                node->element=sum;
                node->right=NULL;
                node->down=NULL;

                // 插入和矩阵S的第i行链表的尾部
                if(S->rhead[i]==NULL){ // 查找第i行的头节点是否为空，是则作为第一个节点插入
                    S->rhead[i]=node;
                    tail=node;
                }else{ // 否则在该列链表的尾部插入
                    tail->right=node;
                    tail=node;
                }
                (S->tu)++; // 增加非零元个数
            }
        }
    }

    // 构造列链表，为每一列维护一个尾指针数组
    OLink *tailCol=(OLink *)malloc(S->nu*sizeof(OLink));
    for(int j=0;j<S->nu;j++){
        tailCol[j]=NULL;
    }        
    // 遍历每一行，将节点插入对应列链表
    for(int i=0;i<S->mu;i++) {
        OLink cur=S->rhead[i];// 读入第i行的第一个节点
        while(cur!=NULL){
            int j=cur->j; // 该节点列号j
            if(S->chead[j]==NULL) { // 查找第j列的头节点是否为空，是则作为第一个节点插入
                S->chead[j]=cur;
                tailCol[j]=cur;
            }else{ // 否则在该列链表的尾部插入
                tailCol[j]->down=cur;
                tailCol[j]=cur;
            }
            cur=cur->right;
        }
    }
    free(tailCol); // 释放列尾指针内存   
    return S;
}

// 打印十字链表稀疏矩阵
void printCrossList(PCrossList S)
{
    OLink last; // 最后一个含非零元的行的最后一个节点
    for(int j=0;j<S->mu;j++){
        OLink q=S->rhead[j];
        while(q!=NULL){
            last=q;
            q=q->right;
        }
    }

    // 遍历打印
    for(int i=0;i<S->mu;i++){
        OLink p=S->rhead[i];
        while(p!=NULL){
            printf("%d %d %d",p->i+1,p->j+1,p->element);
            if(p!=last) printf("\n");
            p=p->right;
        }
    }
}

// 释放链表内存
void freeCrossList(PCrossList M)
{
    for(int i=0;i<M->mu;i++){
        OLink p=M->rhead[i];
        OLink q=NULL;
        while(p!=NULL){
            q=p;
            p=p->right;
            free(q); // 释放节点内存
        }
        
    }
    
    free(M->rhead); // 释放行指针数组内存   
    free(M->chead); // 释放列指针数组内存  
    free(M); // 释放链表内存
}