/*给定两个按照升序排列的有序数组，请把它们合成一个升序数组并输出。*/

#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20 // 定义数组最大长度

// 定义顺序表结构体
typedef struct list{
    int data[2*MAXSIZE]; // 存储数组（输入长度最大MAXSIZE，合并后最大长度需乘2）
    int length; // 数组长度
}SeqList,*PSeqList;

void checkSize(int *n); // 检查数组长度是否在(0,MAXSIZE]范围内
void inputList(PSeqList list); // 输入list存储的data数组
PSeqList mergeList(PSeqList list1,PSeqList list2); // 合并顺序表
void printList(PSeqList list); // 打印顺序表

int main()
{
    int n,m; // 声明数组list1，list2长度变量
    PSeqList list1,list2,list; // 声明顺序表结构体指针

    // 为指针分配内存
    list1=(PSeqList)malloc(sizeof(SeqList));
    list2=(PSeqList)malloc(sizeof(SeqList));
    list=(PSeqList)malloc(sizeof(SeqList));
    if(list1==NULL || list2==NULL || list==NULL){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE); // 退出程序
    }

    // 读取用户输入的list1长度及list1的data数组
    scanf("%d",&n);
    checkSize(&n); // 检查长度
    list1->length=n;
    inputList(list1); // 输入有序循环表数据

    // 读取用户输入的list2长度及list2的data数组
    scanf("%d",&m);
    checkSize(&m); // 检查长度
    list2->length=m; 
    inputList(list2); // 输入有序循环表数据

    // 合并顺序表
    list=mergeList(list1,list2);

    // 打印顺序表
    printList(list);

    // 释放内存
    free(list1);
    free(list2);
    free(list);

    return 0;
}

// 检查数组长度是否在(0,MAXSIZE]范围内
void checkSize(int *n)
{
    while(*n>MAXSIZE||*n<0){
        printf("Please input number that is not more than %d.\n",MAXSIZE);
        getchar(); // 去除可能的换行符
        scanf("%d",n);       
    }
}

// 输入list存储的data数组
void inputList(PSeqList list)
{
    int error=0; // 记录非升序错误
    
    for(int i=0;i<list->length;i++){
        scanf("%d",&list->data[i]);

        if(i>0 && list->data[i]<list->data[i-1]){
            printf("Please enter the array in ascending order.\n");
            error++;
            break;
        }
    }

    while(getchar()!='\n'); // 防止缓冲区存储多余数字，清空多余输入
    
    if(error){ // 如果有升序错误
        inputList(list); // 递归调用inputList()函数，以重新赋值数组
    }
}

// 合并顺序表
PSeqList mergeList(PSeqList list1,PSeqList list2)
{
    PSeqList list; // 声明合并后顺序表
    int i,j; // 声明list1、list2当前脚标
    int k=0; // 定义当前合并表脚标

    list=(PSeqList)malloc(sizeof(SeqList)); // 分配内存
    if(list==NULL){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    for(i=0,j=0;i<list1->length && j<list2->length;){ 
        if(list1->data[i]<list2->data[j]){
            list->data[k]=list1->data[i];
            i++; // list1移动至下一个脚标              
        }else{
            list->data[k]=list2->data[j];
            j++; // list2移动至下一个脚标
        }
        k++; // 合并顺序表移动至下一个脚标
    }

    // 当其中一个表走完后，添加另一个表剩下的数
    if(i==list1->length){
        while(j<list2->length){
            list->data[k++]=list2->data[j++];
        } 
    }else{
        while(i<list1->length){
            list->data[k++]=list1->data[i++];
        } 
    }

    list->length=k; // 赋值顺序表长度

    return list; // 返回顺序表指针
}

// 打印顺序表
void printList(PSeqList list){
    for(int i=0;i<list->length;i++){
        printf("%d",list->data[i]);
        if(i!=list->length-1){ // 非最后一个数据时
            printf("\n"); // 打印换行符
        }
    }
}