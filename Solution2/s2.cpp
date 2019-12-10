// 停车场管理（栈和队列的应用）
#include <iostream>
#define MAXSIZE 100
#define MAXQSIZE 100
using namespace std;

/* 结构体 */
typedef struct                                  // 汽车结构体
{
    char id[10];
    int time;
} Car;

typedef struct                                  // 顺序栈
{
    Car *base;                                  // 栈底指针
    Car *top;                                   // 栈顶指针
    int stacksize;                              // 栈可用的最大容量
}SqStack;                                       

typedef struct QNode                            // 链队
{
    Car data;
    struct QNode *next;
}QNode, *QueuePtr;
typedef struct
{
    QueuePtr front;                             // 队头指针
    QueuePtr rear;                              // 队尾指针
}LinkQueue;

/* 函数声明 */
void 

int main()
{
    int n, q;                                   // 最多可以停放n辆汽车，q个信息


}