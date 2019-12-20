#include <iostream>
#define MAXM 100
#define MAXN 100                                    // m，n的最大值为100，即地图最大为100*100

using namespace std;

typedef struct
{
    int i, j;                                       // (i, j)表示一个坐标                                    
    int d;                                          // d表示走到下一坐标的方向，1上，2下，3左，4右
}Point;

typedef struct
{
    Point *base;
    Point *top;
} SqStack;

Point start = {0, 0, 1};                            // 假设地图起点都为0, 0，起始方向是1 

int map[MAXM][MAXN] = {{0}};                        // 地图数组初始化为0

void FindPath_DFS(Point p);                         // 深度优先寻找路径



void FindPath_DFS(Point p)
{// 深度优先寻找路径

}