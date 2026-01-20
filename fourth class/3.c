/*求任意两个结点间的最短路径长度*/
/*Floyd算法*/
#include <stdio.h>
#include <stdlib.h>
#define MAXVEX 100
#define MAXPATH 10000

// 定义邻接矩阵（无向赋权图）
typedef struct{
    int vexs[MAXVEX]; // 顶点信息
    int arcs[MAXVEX][MAXVEX]; // 边信息
    int arcCount; // 边个数
    int vexCount; // 顶点个数
}Graph,*PGraph;

// 存储最短路径信息结构体
typedef struct{
	int arc[MAXVEX][MAXVEX]; // 存放每对顶点间最短路径长度
	int nextvex[MAXVEX][MAXVEX]; // nextvex[i][j]存放vi到vj最短路径上vi的后继顶点的下标值
}Path;

PGraph readAdjMatrix(int n); // 读取邻接矩阵并存储
void Floyd(PGraph adjMatrix,Path *ppath); // Floyd算法
void printShortestLength(Path *ppath,int paths[][2],int m); // 输出最短路径长度

int main()
{
    int n; // 顶点个数
    int m; // 待求最短路径的结点对个数
    PGraph adjMatrix; // 声明邻接矩阵指针
    scanf("%d",&n);
    getchar(); // 去除换行符

    // 读取邻接矩阵并存储
    adjMatrix=readAdjMatrix(n);

    // 读取待求最短路径的结点对
    scanf("%d",&m);
    int paths[m][2]; // 存储结点对的二维数组
    for(int i=0;i<m;i++){
        scanf("%d %d",&paths[i][0],&paths[i][1]);
        getchar();
    }

    // Floyd算法计算任意两点间最短路径长度
    Path *ppath=(Path *)malloc(sizeof(Path));
    if(!ppath){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    Floyd(adjMatrix,ppath);

    // 输出最短路径长度
    printShortestLength(ppath,paths,m);

    // 释放内存
    free(adjMatrix);
    free(ppath);

    return 0;
}

// 读取邻接矩阵并存储
PGraph readAdjMatrix(int n)
{
    PGraph adjMatrix; // 声明邻接矩阵
    adjMatrix=(PGraph)malloc(sizeof(Graph));
    if(!adjMatrix){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    adjMatrix->vexCount=n; // 赋值顶点个数
    // 赋值顶点信息，顶点集合V
    for(int i=0;i<adjMatrix->vexCount;i++){
        adjMatrix->vexs[i]=i;
    }
    // 赋值边信息
    for(int i=0;i<adjMatrix->vexCount;i++){
        for(int j=0;j<adjMatrix->vexCount;j++){
            scanf("%d",&adjMatrix->arcs[i][j]);
        }
        getchar(); // 去除换行符
    }

    // 返回邻接矩阵指针
    return adjMatrix;
}

// Floyd算法
void Floyd(PGraph adjMatrix,Path *ppath)
{
    // 初始化Path结构体
    for(int i=0;i<adjMatrix->vexCount;i++){ // 遍历所有起点顶点
        for(int j=0;j<adjMatrix->vexCount;j++){ // 遍历所有终点顶点    
            if(adjMatrix->arcs[i][j]!=MAXPATH) // 如果i和j之间有直接路径
                ppath->nextvex[i][j]=j; // 设置i到j的下一个顶点就是j本身
            else 
                ppath->nextvex[i][j]=-1; // 否则设为-1表示不可达
            ppath->arc[i][j]=adjMatrix->arcs[i][j]; // 初始化i到j的路径长度
        }
    }

    // 迭代计算
    for(int k=0;k<adjMatrix->vexCount;k++){ // 中间顶点k
        for(int i=0;i<adjMatrix->vexCount;i++){ // 起点顶点i
            for(int j=0;j<adjMatrix->vexCount;j++){ // 终点顶点j    
                if(ppath->arc[i][k]==MAXPATH || ppath->arc[k][j]==MAXPATH) // 如果i到k或k到j不可达
                    continue; // 跳过此次计算
                if(ppath->arc[i][j]>(ppath->arc[i][k]+ppath->arc[k][j])){ // 如果经过k的路径更短
                    ppath->arc[i][j]=ppath->arc[i][k]+ppath->arc[k][j]; // 更新i到j的最短路径长度
                    ppath->nextvex[i][j]=ppath->nextvex[i][k]; // 更新i到j路径的下一个顶点
                }
            }
        }        
    }    
}

// 输出最短路径长度
void printShortestLength(Path *ppath,int paths[][2],int m)
{
    for(int i=0;i<m;i++){
        printf("%d",ppath->arc[paths[i][0]][paths[i][1]]);
        if(i!=m-1) printf("\n");
    }
}