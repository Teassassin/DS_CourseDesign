#include <cstdio>
#include <iostream>
#include <vector>
#define MAXSIZE 10000
#define MAXM 6 // m是列，n是行
#define MAXN 6 // 地图最大为m*n

using namespace std;

/* 点结构体 */
typedef struct Point
{
    int j, i;   // (j, i)表示一个坐标
    int d;      // d表示走到下一坐标的方向，1上，2下，3左，4右
    int parent; // 广度优先遍历使用，记录上一个通路
} Point;

typedef Point ElemType;

/* 顺序栈 */
typedef struct
{
    Point *base;
    Point *top;
    int stacksize;
} SqStack;
bool InitStack(SqStack &S);               // 构造一个空栈S
bool Push(SqStack &S, ElemType e);        // 插入元素e为新的栈顶元素
bool Pop(SqStack &S, ElemType &e);        // 删除S的栈顶元素，用e返回其值
void GetTop(SqStack S, ElemType *&e);     // 返回S栈顶元素，不修改栈顶指针
bool Empty(SqStack S);                    // 判断栈是否为空
void PrintReverse(SqStack S, ElemType e); // 递归反向打印栈

/* 全局变量 */
Point mapStart = {0, 0, 1, -1};             // 假设地图起点都为0, 0，起始方向是1
Point mapEnd = {MAXN - 1, MAXM - 1, 0, -1}; // 暂定地图终点为MAXM, MAXN

int mazeMap1[MAXN][MAXM] = {
    {0, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0},
    {1, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 1},
    {0, 1, 0, 1, 0, 0}};
int mazeMap2[MAXN][MAXM] = {
    {0, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0},
    {1, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 1},
    {0, 1, 0, 1, 0, 0}};

void FindPath_DFS();               // 深度优先寻找路径
void FindPath_BFS();               // 广度优先寻找路径
bool CheckPoint_BFS(int i, int j); // 判断点是否符合条件

int main()
{
    FindPath_BFS();
    FindPath_DFS();
    return 0;
}

void FindPath_DFS()
{              // 深度优先寻找路径（不需要parent指针）
    SqStack S; // 深度优先遍历用到的栈
    InitStack(S);

    mapStart.d = 1;    // 重置起点信息（一次寻找路径之后d被修改）
    Push(S, mapStart); // 起点进栈
    while (!Empty(S))
    {
        Point *top;
        GetTop(S, top);
        int j = top->j;
        int i = top->i;     // 获取栈顶元素的坐标
        int d = top->d;     // 获取栈顶元素的下一步方向
        mazeMap1[i][j] = 1; // 标记当前位置为1，表示已经走过

        if (top->j == mapEnd.j && top->i == mapEnd.i) // 到达终点
        {
            // 递归打印栈
            Point e;
            PrintReverse(S, e);
            // PrintStack(S);
            cout << "end" << endl;
            return;
        }

        switch (d)
        {
        case 1:
            if (i - 1 >= 0 && mazeMap1[i - 1][j] == 0) // 向上走，判断是否出地图并且当前地图标记为未走过
            {
                Point next = {j, i - 1, 1, -1};
                Push(S, next);
            }
            top->d++; // 当前位置方向+1，即若下一位置无路可走，则当前位置以下一方向继续
            continue;
        case 2:
            if (i + 1 < MAXN && mazeMap1[i + 1][j] == 0) // 向下走
            {
                Point next = {j, i + 1, 1, -1};
                Push(S, next);
            }
            top->d++;
            continue;
        case 3:
            if (j - 1 >= 0 && mazeMap1[i][j - 1] == 0) // 向左走
            {
                Point next = {j - 1, i, 1, -1};
                Push(S, next);
            }
            top->d++;
            continue;
        case 4:
            if (j + 1 < MAXM && mazeMap1[i][j + 1] == 0) // 向右走
            {
                Point next = {j + 1, i, 1, -1};
                Push(S, next);
            }
            top->d++;
            continue;
        }
        Point e;
        Pop(S, e);              // d == 5表示已经无路可走，出栈
        mazeMap1[e.i][e.j] = 0; // 当前位置重置为可达
    }
}

void FindPath_BFS()
{ // 广度优先寻找路径
    // 使用vector模拟队列，不用真队列是因为，pop()之后就找不到走过的通路了
    vector<Point> Q;
    int head = 0;          // 队头
    int rear = 1;          // 队尾
    Q.push_back(mapStart); // 起点入队

    while (!Q.empty())
    {
        Point front;
        front = Q[head];
        int i = front.i;
        int j = front.j;
        mazeMap2[i][j] == 1; // 标记当前位置为已访问
        if (i == mapEnd.i && j == mapEnd.j)
            break; // 找到终点，即为当前head
        head++;    // 队头+1
        if (i - 1 >= 0 && mazeMap2[i - 1][j] == 0)
        {
            Point e = {j, i - 1, 1, head - 1}; // parent指向上一个位置
            Q.push_back(e);
        }
        if (i + 1 < MAXN && mazeMap2[i + 1][j] == 0)
        {
            Point e = {j, i + 1, 1, head - 1}; // 同上
            Q.push_back(e);
        }
        if (j - 1 >= 0 && mazeMap2[i][j - 1] == 0)
        {
            Point e = {j - 1, i, 1, head - 1};
            Q.push_back(e);
        }
        if (j + 1 < MAXM && mazeMap2[i][j + 1] == 0)
        {
            Point e = {j + 1, i, 1, head - 1};
            Q.push_back(e);
        }
    }

    // 用栈保存，再弹出，正好是正序
    SqStack S;
    InitStack(S);
    // 修改d的值，一个p一个q相当于前后指针，用于判断相对位置
    // q走在前面，先到达起点
    Point p = Q[head];
    Point q = Q[p.parent];
    Push(S, p);
    for (; p.parent != -1; p = Q[p.parent], q = Q[q.parent])
    {
        if (q.i == p.i + 1 && q.j == p.j)
            q.d = 1; // 上
        if (q.i == p.i - 1 && q.j == p.j)
            q.d = 2; // 下
        if (q.i == p.i && q.j == p.j + 1)
            q.d = 3; // 左
        if (q.i == p.i && q.j == p.j - 1)
            q.d = 4; // 右
        Push(S, q);  // 改了谁的d就把谁入栈
    }

    // 输出
    Point e;
    Pop(S, e);
    cout << "start";
    while (!Empty(S))
    {
        Pop(S, e);
        if (e.d != 0)
            cout << "->(" << e.j << ", " << e.i << ", " << e.d << ")";
    }
    cout << endl;
}

/* 顺序栈和链队基本操作 */
bool InitStack(SqStack &S)
{                                   // 构造一个空栈S
    S.base = new ElemType[MAXSIZE]; // 为顺序栈动态分配一个最大容量尾MAXSIZE的数组空间
    if (!S.base)
        exit(-1);          // 存储分配失败
    S.top = S.base;        // top初始为base，空栈
    S.stacksize = MAXSIZE; // stacksize置为栈的最大容量MAXSIZE
    return true;
}

bool Push(SqStack &S, ElemType e)
{ // 插入元素e为新的栈顶元素
    if (S.top - S.base == S.stacksize)
        return false; // 栈满
    *S.top++ = e;     // 元素e压入栈顶，栈顶指针加1
    return true;
}

bool Pop(SqStack &S, ElemType &e)
{ // 删除S的栈顶元素，用e返回其值
    if (S.top == S.base)
        return false; // 栈空
    e = *--S.top;     // 栈顶指针减1，将栈顶元素赋给e
    return true;
}

void GetTop(SqStack S, ElemType *&e)
{                        // 返回S的栈顶元素，不修改栈顶指针
    if (S.top != S.base) // 栈非空
        e = (S.top - 1); // 返回栈顶元素的指针
}

bool Empty(SqStack S)
{ // 判断栈是否为空
    if (S.top == S.base)
        return true;
    return false;
}

void PrintReverse(SqStack S, ElemType e)
{ // 递归反向打印栈
    if (!Empty(S))
    {
        Pop(S, e);
        PrintReverse(S, e);
    }
    else
        return;
    // 由于算法的局限性，最后我们需要把每个点的d（方向）减1
    cout << '(' << e.j << ", " << e.i << ", " << e.d - 1 << ")->";
}
