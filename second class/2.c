/*稀疏矩阵加法*/

#include <stdio.h>
#define MAXIMUM 50

// 定义三元组结构体
typedef struct triple{
    int i,j; // 行、列
    int element; // 元素
}Triple;

// 定义稀疏矩阵结构体
typedef struct matrix{
    Triple data[MAXIMUM*MAXIMUM]; // 三元组结构体数组
    int mu,nu,tu; // 行数、列数、非零元个数
}SparseMatrix;

void readSparseMatrix(SparseMatrix* M1,SparseMatrix* M2); // 读取稀疏矩阵三元组
void addSparseMatrix(SparseMatrix M1,SparseMatrix M2,SparseMatrix* S); // 稀疏矩阵相加
void printSparseMatrix(SparseMatrix T); // 打印矩阵

int main()
{
    SparseMatrix M1,M2,S; // 稀疏矩阵结构体声明（原始矩阵及和矩阵）
    int m,n,t1,t2; // 稀疏矩阵行数、列数、M1非零元个数、M2非零元个数声明
    
    scanf("%d %d %d %d",&m,&n,&t1,&t2); // 读取行数、列数、非零元个数
    getchar(); // 去除换行符

    // 赋值行数、列数、非零元个数
    M1.mu=M2.mu=m;
    M1.nu=M2.nu=n;
    M1.tu=t1;
    M2.tu=t2;

    readSparseMatrix(&M1,&M2); // 读取两个稀疏矩阵

    addSparseMatrix(M1,M2,&S); // 稀疏矩阵相加并赋值给S
    
    printSparseMatrix(S); // 打印稀疏矩阵

    return 0;
}

// 读取稀疏矩阵三元组
void readSparseMatrix(SparseMatrix* M1,SparseMatrix* M2)
{
    int cur1,cur2; // 当前三元组数组脚标

    for(cur1=0;cur1<M1->tu;cur1++){
        scanf("%d %d %d",&(M1->data[cur1].i),&(M1->data[cur1].j),&(M1->data[cur1].element));
        getchar(); // 清除换行符
    }

    for(cur2=0;cur2<M2->tu;cur2++){
        scanf("%d %d %d",&(M2->data[cur2].i),&(M2->data[cur2].j),&(M2->data[cur2].element));
        getchar(); // 清除换行符
    }
}

// 稀疏矩阵相加
void addSparseMatrix(SparseMatrix M1,SparseMatrix M2,SparseMatrix* S)
{
    int p1=0,p2=0,p3=0; // 初始化M1、M2、A当前三元组脚标
    // 赋值和矩阵行数、列数
    S->mu=M1.mu;
    S->nu=M1.nu;

    while(p1<M1.tu && p2<M2.tu){
        if(M1.data[p1].i<M2.data[p2].i || 
            M1.data[p1].i==M2.data[p2].i && M1.data[p1].j<M2.data[p2].j){
                S->data[p3++]=M1.data[p1++]; // M1位置靠前时直接赋值给S并后移位置
        }else if(M1.data[p1].i>M2.data[p2].i || 
            M1.data[p1].i==M2.data[p2].i && M1.data[p1].j>M2.data[p2].j){
                S->data[p3++]=M2.data[p2++]; // M2位置靠前时直接赋值给S并后移位置
        }else{ // M1和M2的三元组位置相同
            int sum=M1.data[p1].element+M2.data[p2].element; // 求和
            if(sum!=0){ // 和非0时赋值给S
                S->data[p3].i=M1.data[p1].i;
                S->data[p3].j=M1.data[p1].j;
                S->data[p3].element=sum;
                p3++; // 后移位置
            }
            p1++; // 后移位置
            p2++; // 后移位置
        }
    }

    // 处理剩余三元组
    while(p1<M1.tu) S->data[p3++]=M1.data[p1++];
    while(p2<M2.tu) S->data[p3++]=M2.data[p2++];

    S->tu=p3; // 赋值和矩阵非零元个数
}

// 打印矩阵
void printSparseMatrix(SparseMatrix T)
{
    for(int s=0;s<T.tu;s++){
        printf("%d %d %d",T.data[s].i,T.data[s].j,T.data[s].element);
        if(s!=T.tu-1){
            printf("\n");
        }
    }
}