/*稀疏矩阵乘法*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXIMUM 100

// 定义三元组结构体
typedef struct triple{
    int i,j; // 行、列
    int element; // 元素
}Triple;

// 定义稀疏矩阵结构体
typedef struct matrix{
    Triple data[MAXIMUM]; // 三元组结构体数组
    int mu,nu,tu; // 行数、列数、非零元个数
    int rpos[MAXIMUM]; // 每一行第一个非零元在data数组中的位置
}SparseMatrix;

void readSparseMatrix(SparseMatrix* M); // 读取稀疏矩阵三元组
void multiplySparseMatrix(SparseMatrix M1,SparseMatrix M2,SparseMatrix *S); // 稀疏矩阵乘法
void printSparseMatrix(SparseMatrix T); // 打印矩阵

int main()
{
    SparseMatrix M1,M2,S; // 稀疏矩阵结构体声明（原始矩阵及乘积矩阵）

    // 读取稀疏矩阵
    readSparseMatrix(&M1);
    readSparseMatrix(&M2);

    // 判断能否进行矩阵乘法
    if(M1.nu!=M2.mu){
        printf("Please enter two matrices with the former's column equal to the latter's row.\n");
        exit(EXIT_FAILURE);
    }

    // 稀疏矩阵乘法
    multiplySparseMatrix(M1,M2,&S);

    // 打印矩阵
    printSparseMatrix(S);

    return 0;   
}

// 读取稀疏矩阵三元组
void readSparseMatrix(SparseMatrix* M)
{
    int m,n; // 稀疏矩阵行数、列数声明
    int num[MAXIMUM]; // 每行非零元个数
    scanf("%d %d",&m,&n); // 读取行数、列数
    getchar();
    // 赋值
    M->mu=m;
    M->nu=n;

    M->tu=0;
    while(1){
        scanf("%d %d %d",&M->data[M->tu].i,&M->data[M->tu].j,&M->data[M->tu].element);
         // 遇到全0时停止
        if(M->data[M->tu].i==0 && M->data[M->tu].j==0 && M->data[M->tu].element==0)
            break;
        
        M->data[M->tu].i--;
        M->data[M->tu].j--;
       
        // 检查边界
        if(M->data[M->tu].i>=M->mu || M->data[M->tu].j>=M->nu){
            printf("Invalid position!\n");
            exit(EXIT_FAILURE);
        }
        M->tu++;
        if(M->tu>=MAXIMUM) {
            printf("Too many non-zero elements!\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int row=0;row<M->mu;row++){
        num[row]=0; // 初始化数组
    }

    for(int p=0;p<M->tu;p++){
        num[M->data[p].i]++; // 累加计算每行非零元个数
    }

    M->rpos[0]=0;
    for(int q=1;q<=M->mu;q++){
        M->rpos[q]=M->rpos[q-1]+num[q-1]; // 记录rpos
    }
}

// 稀疏矩阵乘法
void multiplySparseMatrix(SparseMatrix M1,SparseMatrix M2,SparseMatrix *S)
{
    int row1,row2,col,p,q;
    int temp[MAXIMUM];

    S->mu=M1.mu;
    S->nu=M2.nu;
    S->tu=0; // 非零元个数初始化为0
    memset(S->rpos,0,sizeof(S->rpos));

    if(M1.tu*M2.tu!=0){
        for(row1=0;row1<M1.mu;row1++){ // M1中各行
            memset(temp,0,M2.nu*sizeof(int)); // 乘法前置0
            S->rpos[row1]=S->tu; // 第row1行第一个非零元的位置

            // 遍历M1第row1行非零元
            for(p=M1.rpos[row1];p<M1.rpos[row1+1];p++){
                int col1=M1.data[p].j; // 当前非零元素所在列
                row2=col1;
                // 将M1中元素与M2对应元素相乘
                for(q=M2.rpos[row2];q<M2.rpos[row2+1];q++){
                    col=M2.data[q].j;
                    temp[col]+=M1.data[p].element*M2.data[q].element;
                }             
            }

            // 将temp的非零结果存入S的data中
            for(col=0;col<S->nu;col++){
                if(temp[col]!=0){
                    if(S->tu>=MAXIMUM) exit(EXIT_FAILURE);
                    S->data[S->tu].i=row1;
                    S->data[S->tu].j=col;
                    S->data[S->tu].element=temp[col];
                    S->tu++;
                }
            }
        }
        S->rpos[S->mu]=S->tu; // 最后标记矩阵结束位置
    }
}

// 打印矩阵
void printSparseMatrix(SparseMatrix T)
{
    for(int s=0;s<T.tu;s++){
        T.data[s].i++;
        T.data[s].j++;
        printf("%d %d %d",T.data[s].i,T.data[s].j,T.data[s].element);
        if(s!=T.tu-1){
            printf("\n");
        }
    }
}