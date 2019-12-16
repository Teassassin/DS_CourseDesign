/*
 * 停车场管理（栈和队列的应用）
 * by TKK
 * Created 2019/12/09
 * Ended 2019/12/12
 */

#include <iostream>
#include <cstring>
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
} SqStack;                                       

typedef struct QNode                            // 链队
{
    Car data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct
{
    QueuePtr front;                             // 队头指针
    QueuePtr rear;                              // 队尾指针
} LinkQueue;

/* 顺序栈 */
bool InitStack(SqStack &S);                     // 构造一个空栈S
bool Push(SqStack &S, Car e);                   // 插入元素e为新的栈顶元素
bool Pop(SqStack &S, Car &e);                   // 删除S的栈顶元素，用e返回其值
Car GetTop(SqStack S);                          // 返回S栈顶元素，不修改栈顶指针

/* 链队 */
bool InitQueue(LinkQueue &Q);                   // 构造一个空队列Q
bool EnQueue(LinkQueue &Q, Car e);              // 插入元素e为Q的新的队尾元素
bool DeQueue(LinkQueue &Q, Car &e);             // 删除Q的队头元素，用e返回其值
Car GetHead(LinkQueue Q);                       // 返回Q的队头元素，不修改队头指针


int main()
{
    int n, q;                                   // 最多可以停放n辆汽车，q个信息
    int clock = 0;                              // 总时间
    int count_s = 0, count_q = 0;               // 栈和队列计数共多少车辆
    SqStack S;                                  
    LinkQueue Q;                                
    InitQueue(Q);                               // 初始化栈S
    InitStack(S);                               // 初始化队列

    cin >> n >> q;                              // 停车场最多可以停放n辆汽车
    S.stacksize = n;                            // 修改栈的最大空间

    for (int i = 0; i < q; i++)
    {
        char status[10];                        // 汽车"arrive"或"leave"
        Car _car;                               // 待操作汽车
        cin >> status >> _car.id >> _car.time;  // 输入id和到达或离去的时间点
        clock = _car.time;
        if (!strcmp(status, "arrive"))
        {
            if(Push(S, _car))
            {
                cout << "Stack " << ++count_s << endl;
                continue;
            }
            else
            {
                EnQueue(Q, _car);
                cout << "Queue " << ++count_q << endl;
            }
        }
        else if (!strcmp(status, "leave"))
        {
            // 车在便道
            bool flag = false;                  // 若车在便道上，则改为true
            QueuePtr p = Q.front;               // p指向front，遍历队列
            while (p != Q.rear)                 
            {
                if (!strcmp(p->next->data.id, _car.id))
                {
                    if (count_q == 1)           // 如果队只剩一个结点
                    {
                        delete Q.rear;          // 删除该结点
                        Q.rear = Q.front;
                        count_q--;              // 与下面相同的步骤
                        flag = true;
                        cout << 0 << endl;
                        break;
                    }
                    if (Q.rear == p->next)      // 如果要删除的结点恰好是尾结点
                        Q.rear = p;
                    QueuePtr q = p->next;       // 临时指针
                    p->next = p->next->next;    // 删除该结点
                    delete q;                   // 释放空间，车走了
                    q = NULL;
                    count_q--;                  // 队列数减1
                    flag = true;                // 更改标记为true
                    cout << 0 << endl;
                    break;
                }
                p = p->next;
            }
            if (flag)
                continue;

            // 车在停车场
            SqStack _S;                         // 临时栈供停车场的车让路
            InitStack(_S);                      // 初始化临时栈
            Car e;                              // 中间变量，临时存放车辆
            int time;                           // 停留时间
            while (strcmp(GetTop(S).id, _car.id))
            {                                   // 出栈、入临时栈
                Pop(S, e);                      // 出栈，出停车场
                cout << e.id << ":out ";        // 模拟出停车场
                Push(_S, e);                    // 入临时栈
            }

            Pop(S, e);                          // 指定车辆出栈
            cout << e.id << ":out ";            // 模拟出停车场
            time = clock - e.time;              // 计算停留时间

            while (Pop(_S, e))
            {                                   // 不断循环将临时栈的车全部弹出
                cout << e.id << ":in ";         // 模拟进入停车场
                Push(S, e);                     // 入栈，进入停车场
            }
            if(DeQueue(Q, e))                   // 如果便道上有车
            {                                   // 出队，便道上等待的车可以进停车场了
                cout << e.id << ":in ";         // 模拟进入停车场
                e.time = clock;                 // 更新车辆时间，以当前时间作为该车的初始时间，注意先改时间再入栈
                count_q--;                      // 队列总数减1，栈总数不变
                Push(S, e);                     // 入栈，便道上的车进入停车场
            }
            else                                // 如果便道上没有车
                count_s--;                      // 栈总数减1

            cout << time << endl;               // 输出停留时间
        }
    }

    return 0;
}


/* 顺序栈和链队基础操作 */
bool InitStack(SqStack &S)
{// 构造一个空栈S
    S.base = new Car[MAXSIZE];                  // 为顺序栈动态分配一个最大容量尾MAXSIZE的数组空间
    if (!S.base)
        exit(-1);                               // 存储分配失败
    S.top=S.base;                               // top初始为base，空栈
    S.stacksize = MAXSIZE;                      // stacksize置为栈的最大容量MAXSIZE
    return true;
}

bool Push(SqStack &S, Car e)
{// 插入元素e为新的栈顶元素
    if (S.top - S.base == S.stacksize)
        return false;                           // 栈满
    *S.top++ = e;                               // 元素e压入栈顶，栈顶指针加1
    return true;
}

bool Pop(SqStack &S, Car &e)
{// 删除S的栈顶元素，用e返回其值
    if (S.top == S.base)
        return false;                           // 栈空
    e = *--S.top;                               // 栈顶指针减1，将栈顶元素赋给e
    return true;
}

Car GetTop(SqStack S)
{// 返回S的栈顶元素，不修改栈顶指针
    if (S.top != S.base)                        // 栈非空
        return *(S.top - 1);                    // 返回栈顶元素的值，栈顶指针不变
}

bool InitQueue(LinkQueue &Q)
{// 构造一个空队列Q
    Q.front = Q.rear = new QNode;               // 生成新结点作为头结点，队头和队尾指针指向此结点
    Q.front->next = NULL;                       // 头结点的指针域置空
    return true;
}

bool EnQueue(LinkQueue &Q, Car e)
{// 插入元素e为Q的新的队尾元素
    QueuePtr p = new QNode;                     // 为入队元素分配结点空间，用指针p指向
    p->data = e;                                // 将新结点数据域置为e
    p->next = NULL;
    Q.rear->next = p;                           // 将新结点插入到队尾
    Q.rear = p;                                 // 修改队尾指针
    return true;
}

bool DeQueue(LinkQueue &Q, Car &e)
{// 删除Q的队头元素，用e返回其值
    if (Q.front == Q.rear)
        return false;                           // 队列为空
    QueuePtr p = Q.front->next;                 // p指向队头元素
    e = p->data;                                // e保存队头元素的值
    Q.front->next = p->next;                    // 修改头结点的指针域
    if (Q.rear == p)
        Q.rear = Q.front;                       // 最后一个元素被删，队尾指针指向头结点
    delete p;                                   // 释放原队头元素的空间
    return true;
}

Car GetHead(LinkQueue Q)
{// 返回Q的队头元素，不修改队头指针
    if (Q.front != Q.rear)
        return Q.front->next->data;             // 返回队头元素的值，队头指针不变
}