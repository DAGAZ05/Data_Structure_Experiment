/*稀疏矩阵转置*/

#include <stdio.h>
#define MAXIMUM 20

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

void readSparseMatrix(SparseMatrix* m); // 读取稀疏矩阵三元组
void transposeSparseMatrix(SparseMatrix M,SparseMatrix* T); // 转置稀疏矩阵（快速置换法）
void printTSparseMatrix(SparseMatrix T); // 打印转置后矩阵

int main()
{
    int n,m; // 稀疏矩阵行数、列数声明
    SparseMatrix M,T; // 稀疏矩阵结构体声明（原始矩阵及转置矩阵）

    scanf("%d %d",&n,&m); // 读取行数、列数
    while(n>MAXIMUM || m>MAXIMUM){
        printf("Please enter n not more than %d and m not more than %d.\n",MAXIMUM,MAXIMUM);
        scanf("%d %d",&n,&m); // 重新读取
    }  
    getchar(); // 去除换行符

    // 赋值行数、列数
    M.mu=n;
    M.nu=m;

    readSparseMatrix(&M); // 读取稀疏矩阵三元组

    transposeSparseMatrix(M,&T); // 转置稀疏矩阵

    printTSparseMatrix(T); // 打印转置矩阵

    return 0;
}

// 读取稀疏矩阵三元组
void readSparseMatrix(SparseMatrix* m)
{
    int cur=-1; // 当前三元组数组脚标
    
    do{
        cur++;
        scanf("%d %d %d",&(m->data[cur].i),&(m->data[cur].j),&(m->data[cur].element));
        getchar();  
    }while(cur<m->mu*m->nu && (m->data[cur].i || m->data[cur].j || m->data[cur].element));

    m->tu=cur; // 赋值非零元个数
}

// 转置稀疏矩阵（快速置换法）
void transposeSparseMatrix(SparseMatrix M,SparseMatrix* T)
{
    int col;
    // 赋值
    T->mu=M.mu;
    T->nu=M.nu;
    T->tu=M.tu;
    int num[T->nu]; // 每列的非零元个数数组
    int cpot[T->nu]; // 每列第一个非零元的位置

    if(T->tu){
        for(col=0;col<T->nu;col++){
            num[col]=0; // 初始化数组
        }

        for(int p=0;p<T->tu;p++){
            num[M.data[p].j]++; // 累加计算每列非零元个数
        }

        cpot[0]=0;
        for(int q=1;q<T->nu;q++){
            cpot[q]=cpot[q-1]+num[q-1]; // 记录位置信息
        }

        // 转置矩阵赋值
        for(int r=0;r<T->tu;r++){
            col=M.data[r].j; // 当前位置（原矩阵）的列数
            T->data[cpot[col]].j=M.data[r].i;
            T->data[cpot[col]].i=M.data[r].j;
            T->data[cpot[col]].element=M.data[r].element;
            cpot[col]++; // 该列的位置信息后移
        }
    }
}

// 打印转置后矩阵
void printTSparseMatrix(SparseMatrix T)
{
    for(int s=0;s<T.tu;s++){
        printf("%d %d %d",T.data[s].i,T.data[s].j,T.data[s].element);
        if(s!=T.tu-1){
            printf("\n");
        }
    }
}