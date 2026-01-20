/*求0号结点到其余所有结点的最短路径的长度*/
/*Dijkstra算法*/
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

// 存放v0到其余各顶点的最短路径及长度
typedef struct{
    int vertex; // 顶点信息
    int length; // 最短路径长度
    int prevex; // v0到vi时vi的前驱顶点
}Path;

PGraph readAdjMatrix(int n); // 读取邻接矩阵并存储
void Dijkstra(PGraph adjMatrix,Path dist[]); // Dijkstra算法
void printShortestPath(int n,Path dist[]); // 输出最短路径长度

int main()
{
    int n; // 顶点个数
    PGraph adjMatrix; // 声明邻接矩阵指针   
    scanf("%d",&n);
    getchar(); // 去除换行符
    Path dist[n]; // 存储最短路径信息的结构体数组

    // 读取邻接矩阵并存储
    adjMatrix=readAdjMatrix(n);

    // Dijkstra算法计算最短路径
    Dijkstra(adjMatrix,dist);

    // 输出最短路径长度
    printShortestPath(n,dist);

    // 释放内存
    free(adjMatrix);

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

// Dijkstra算法
void Dijkstra(PGraph adjMatrix,Path dist[])
{
    int minvex; // 距离值最小的顶点
    int min; // 距离值最小时边的长度

    // 初始化dist[]数组首项，将v0放入集合U
    dist[0].length=0;
    dist[0].prevex=0;
    dist[0].vertex=adjMatrix->vexs[0];
    adjMatrix->arcs[0][0]=1;
    // 初始化dist[]
    for(int i=1;i<adjMatrix->vexCount;i++){
        dist[i].vertex=adjMatrix->vexs[i];
        dist[i].length=adjMatrix->arcs[0][i]; // 初始集合V-U中顶点的距离值
        if(dist[i].length!=MAXPATH)
            dist[i].prevex=0;
        else
            dist[i].prevex=-1;
    }

    for(int i=1;i<adjMatrix->vexCount;i++){
        min=MAXPATH;
        minvex=0;
        // 在V-U中选出距离值最小的顶点
        for(int j=1;j<adjMatrix->vexCount;j++){
            if(adjMatrix->arcs[j][j]==0 && dist[j].length<min){
                min=dist[j].length;
                minvex=j;
            }
        }

        if(minvex==0) // 从v0没有路径可以通往集合V-U中的顶点
 		    break;

        adjMatrix->arcs[minvex][minvex]=1; // 标记minvex顶点进入集合U

        // 调整集合V-U中的顶点的最短路径
        for(int j=1;j<adjMatrix->vexCount;j++){
            // 跳过集合U中的顶点
            if(adjMatrix->arcs[j][j]==1) continue;
            if(dist[j].length > dist[minvex].length+adjMatrix->arcs[minvex][j]){
                dist[j].length=dist[minvex].length+adjMatrix->arcs[minvex][j];
                dist[j].prevex=minvex;
            }
        }
    }
}

// 输出最短路径长度
void printShortestPath(int n,Path dist[])
{
    for(int i=0;i<n;i++){
        printf("%d",dist[i].length);
        if(i!=n-1) printf("\n");
    }
}