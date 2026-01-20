/*限制使用双向链表作存储结构，请根据用户输入的一个整数
（该整数表示精确到小数点后的位数，可能要求精确到小数点后 500 位），
高精度计算PI值。可以利用反三角函数幂级展开式来进行计算。*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 500 // 定义最大执行位数

// 定义双向链表结点
typedef struct node{
    int data; // 存储一位数字数据
    struct node *prev; // 前驱指针
    struct node *next; // 后继指针
}doubleNode,*PDoubleNode;

// 定义双向链表
typedef struct list{
    PDoubleNode head; // 双向链表头指针
    PDoubleNode tail; // 双向链表尾指针
    int length; // 链表长度
}doubleList,*PDoubleList;

PDoubleList initialList(int num); // 初始化链表指针
int calculateTerm(int n); // 计算项数
void insertAtTail(PDoubleList list,int data); // 尾部插入结点
void multipleList(PDoubleList list,int num); // 链表数乘
void divideList(PDoubleList list,int num); // 链表除法
void addList(PDoubleList current,PDoubleList sum); // 链表加法
void printList(PDoubleList list,int n); // 打印数据
void destroyList(PDoubleList list); // 销毁链表及结点指针

int main()
{
    int n; // 声明读取的小数点后位数
    int m; // 声明计算的展开式项数
    PDoubleList sum,cur; // 分别声明最终链表结果及展开式当前项的链表结果
    
    // 读取\pi小数点后位数
    scanf("%d",&n); 
    while(n>MAXSIZE || n<0){ // 读取位数超出最大执行范围
        getchar();
        printf("Please enter number no more than %d.\n",MAXSIZE);
        scanf("%d",&n);
    }

    // 计算项数
    m=calculateTerm(n);

    // 初始化并赋值第一项（整数部分）
    sum=initialList(2);
    cur=initialList(2);

    // 延长链表以存储n位数
    for(int i=0;i<n;i++){
        insertAtTail(sum,0);
        insertAtTail(cur,0);
    }

    // 按展开项递推及求和
    for(int i=1,j=3;i<=m;i++,j+=2){
        multipleList(cur,i);
        divideList(cur,j);
        addList(cur,sum);
    }

    // 打印和表数据
    printList(sum,n);

    // 销毁链表及结点指针
    destroyList(cur);
    destroyList(sum);

    return 0;
}

// 计算项数
int calculateTerm(int n)
{
    int k=1; // 项数起始定义
    double term,sum=0; // 声明当前项、总和

    // 使用展开式余项判断项数
    while(sum<=n+1){
        term=1.0*(2*k+1)/k;
        sum+=log10(term);
        k++;
    }
    return (k-1);
}

// 初始化链表指针，并赋值第一项
PDoubleList initialList(int num)
{
    PDoubleList list=(PDoubleList)malloc(sizeof(doubleList)); // 分配内存
    if(list==NULL){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE); // 退出程序
    }

    PDoubleNode node=(PDoubleNode)malloc(sizeof(doubleNode)); // 分配内存
    if(node==NULL){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE); // 退出程序
    }

    PDoubleNode nodeFirst=(PDoubleNode)malloc(sizeof(doubleNode)); // 分配内存
    if(nodeFirst==NULL){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE); // 退出程序
    }

    list->head=node; // 头指针
    list->tail=nodeFirst; // 尾指针
    
    // 头结点赋值
    node->next=nodeFirst;
    node->prev=nodeFirst;

    // 第一项赋值
    nodeFirst->data=num;
    nodeFirst->next=node;
    nodeFirst->prev=node;

    return list;
}

// 尾部插入结点
void insertAtTail(PDoubleList list,int data)
{
    PDoubleNode pre,cur; // 声明原来最后的结点、添加的结点

    pre=list->tail;
    if(pre==NULL){ // 判断链表是否被创建
        printf("You haven't create a double linked list.\n");
        return;
    }

    cur=(PDoubleNode)malloc(sizeof(doubleNode)); // 分配新结点内存
    if(cur==NULL){ // 内存分配失败
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE); // 退出程序
    }

    cur->data=data;
    cur->next=pre->next;
    cur->prev=pre;
    pre->next->prev=cur;
    pre->next=cur;
    list->tail=cur;
}

// 链表数乘
void multipleList(PDoubleList list,int num)
{
    int temp,carry=0; // 定义暂存数据、小数点后部分进位的值
    PDoubleNode cur; // 声明当前结点
    cur=list->tail; // 当前结点指向链表尾指针
    
    while(cur!=list->head->next){ // 小数部分数乘
        temp=cur->data*num+carry;
        cur->data=temp%10; // 更新结点数据
        carry=temp/10; // 进位
        cur=cur->prev; // 向前移动指针
    }
    // 头结点后的下一个结点存储整数部分，为防止继续进位，故不对其取模
    cur->data=cur->data*num+carry; 
}

// 链表除法
void divideList(PDoubleList list,int num)
{
    int temp,rem=0; // 定义暂存数据、做除法后的余数
    PDoubleNode cur; // 声明当前结点
    cur=list->head->next; // 当前结点指向链表第一个有效结点（整数存储）
    
    while(cur!=list->head){
        temp=cur->data+rem*10;
        rem=temp%num; // 更新余数
        cur->data=temp/num; // 更新当前结点数据
        cur=cur->next; // 向后移动指针
    }
}

// 链表加法
void addList(PDoubleList current,PDoubleList sum)
{
    int temp,carry=0; // 定义暂存数据、小数点后部分进位的值
    PDoubleNode cur1,cur2; // 声明项表、和表当前结点
    cur1=current->tail; // 指向尾指针
    cur2=sum->tail; // 指向尾指针

    while(cur1!=current->head->next){ // 小数部分加法
        temp=cur1->data+cur2->data+carry;
        cur2->data=temp%10; // 更新和表数据
        carry=temp/10; // 更新进位
        cur1=cur1->prev; // 向前移动指针
        cur2=cur2->prev; // 向前移动指针
    }
    // 整数部分加法（不取模）
    cur2->data=cur1->data+cur2->data+carry;
}

// 打印数据
void printList(PDoubleList list,int n)
{
    PDoubleNode cur; // 当前结点
    int count=0; // 计数器
    
    cur=list->head->next; // 当前结点指针指向第一个有效结点（整数部分）  
    printf("%d.",cur->data); // 打印整数部分及小数点
    cur=cur->next; // 指针后移
    
    while(cur!=list->head && count<n){
        printf("%d",cur->data);
        cur=cur->next; // 指针后移
        count++; // 计数器自增
    }
    printf("\n");
}

// 销毁链表及结点指针
void destroyList(PDoubleList list)
{
    PDoubleNode prev=NULL,cur; // 前结点、当前结点指针
    cur=list->head->next; // 当前结点指向头结点的下一个结点
    while(cur!=list->head){
        prev=cur; // 前结点指向当前结点
        cur=cur->next; // 移动至下一个结点
        free(prev); // 释放结点内存
    }
    free(list->head); // 释放头结点内存
    free(list); // 释放链表指针内存
}