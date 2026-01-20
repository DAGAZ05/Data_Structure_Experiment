/*设A与B分别为两个带有头结点的有序循环链表
（所谓有序是指链接点按数据域值大小链接，本题不妨设按数据域值从小到大排列），
list1和list2分别为指向两个链表的指针。
请写出将这两个链表合并为一个带头结点的有序循环链表的算法。
1) 从小到大；
2) 从大到小。*/

#include <stdio.h>

typedef struct node{
    int data;
    struct node *next;
}Node,*PNode;

typedef struct list{
    PNode head;
}List,*PList;



int main()
{
    PList list1,list2;
}
