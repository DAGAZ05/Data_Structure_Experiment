/*求给定两个结点间的最短路径*/
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

// 存放第一个给定点到其余各顶点的最短路径及长度
typedef struct{
    int vertex; // 顶点信息
    int length; // 最短路径长度
    int prevex; // 第一个给定点到vi时vi的前驱顶点
}Path;

PGraph readAdjMatrix(int n); // 读取邻接矩阵并存储
void Dijkstra(PGraph adjMatrix,Path dist[],int start); // Dijkstra算法
void printShortestPath(Path dist[],int n,int start,int end); // 输出两个给定结点间的最短路径

int main()
{
    int n; // 顶点个数
    int i,j; // 给定结点
    PGraph adjMatrix; // 声明邻接矩阵指针   
    scanf("%d",&n);
    getchar(); // 去除换行符
    Path dist[n]; // 存储最短路径信息的结构体数组
    
    // 读取邻接矩阵并存储
    adjMatrix=readAdjMatrix(n);

    scanf("%d %d",&i,&j); // 读取给定结点

    // Dijkstra算法计算最短路径
    Dijkstra(adjMatrix,dist,i);

    // 输出两个给定结点间的最短路径
    printShortestPath(dist,n,i,j);

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
void Dijkstra(PGraph adjMatrix,Path dist[],int start)
{
    int minvex; // 距离值最小的顶点
    int min; // 距离值最小时边的长度

    // 初始化dist[]数组首项，将第一个给定结点放入集合U
    dist[start].length=0;
    dist[start].prevex=start;
    dist[start].vertex=adjMatrix->vexs[start];
    adjMatrix->arcs[start][start]=1;
    // 初始化dist[]
    for(int i=0;i<adjMatrix->vexCount;i++){
        if(i==start) continue; // 跳过第一个给定结点
        dist[i].vertex=adjMatrix->vexs[i];
        dist[i].length=adjMatrix->arcs[start][i]; // 初始集合V-U中顶点的距离值
        if(dist[i].length!=MAXPATH)
            dist[i].prevex=start;
        else
            dist[i].prevex=-1;
    }

    for(int i=0;i<adjMatrix->vexCount;i++){
        if(i==start) continue; // 跳过第一个给定结点
        min=MAXPATH;
        minvex=start;
        // 在V-U中选出距离值最小的顶点
        for(int j=0;j<adjMatrix->vexCount;j++){
            if(j==start) continue; // 跳过第一个给定结点
            if(adjMatrix->arcs[j][j]==0 && dist[j].length<min){
                min=dist[j].length;
                minvex=j;
            }
        }

        if(minvex==start) // 从第一个给定结点没有路径可以通往集合V-U中的顶点
 		    break;

        adjMatrix->arcs[minvex][minvex]=1; // 标记minvex顶点进入集合U

        // 调整集合V-U中的顶点的最短路径
        for(int j=0;j<adjMatrix->vexCount;j++){
            // 跳过集合U中的顶点
            if(adjMatrix->arcs[j][j]==1) continue;
            if(dist[j].length > dist[minvex].length+adjMatrix->arcs[minvex][j]){
                dist[j].length=dist[minvex].length+adjMatrix->arcs[minvex][j];
                dist[j].prevex=minvex;
            }
        }
    }
}

// 输出两个给定结点间的最短路径
void printShortestPath(Path dist[],int n,int start,int end)
{
    int path[n]; // 存储最短路径数组
    int i,j,k;
    for(i=end,j=0;;i=dist[i].prevex,j++){
        path[j]=i; // 逆序存储path
        if(i==start) break;
    }
    for(k=j;k>=0;k--){
        printf("%d",path[k]); // 顺序打印
        if(k!=0) printf("\n");
    }
}