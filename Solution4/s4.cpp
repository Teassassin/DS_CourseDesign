/*
 * 校园导游咨询（图的应用）
 * by TKK
 * Created 2019/12/16
 * Ended 2019/12/16
 */

#include <iostream>
#include <cstring>
#define OK 1
#define ERROR 0
#define OVERFLOW -2
typedef int Status;
#define MaxInt 32767                                // 表示极大值，即∞
#define MVNum 20                                    // 最大顶点数

using namespace std;

typedef struct
{
    char name[50];                                  // 名称
    char code[10];                                  // 代号
    char dec[200];                                  // 简介
    int id;                                         // 编号
} VerTexType;
typedef int ArcType;                                // 假设边的权值类型为整型
typedef struct
{
    VerTexType vexs[MVNum];                         // 顶点表
    ArcType arcs[MVNum][MVNum];                     // 邻接矩阵
    int vexnum, arcnum;                             // 图的当前点数和边数
} AMGraph;

Status CreateUDN(AMGraph &G);                       // 邻接矩阵表示法，创建无向网G
int LocateVexById(AMGraph G, int _id);              // 根据景点编号定位顶点下标
int LocateVexByName(AMGraph G, char *_name);        // 根据景点名称定为顶点下标
void ShortestPath_Floyd(AMGraph G);                 // 用Floyd算法求无向网G中各对顶点i和j之间的最短路径
void PrintPath(AMGraph G, int _i, int _j);          // 打印两地之间的路线
void Query1(AMGraph G);                             // 询问景点信息
void Query2(AMGraph G);                             // 路径查询，两景点的最短路径及长度
void Modify(AMGraph &G);                            // 修改景点信息

ArcType dist[MVNum][MVNum];                         // 最短路径矩阵
ArcType path[MVNum][MVNum];                         // 各个顶点到其余顶点前驱顶点位置                 


int main()
{
    AMGraph G;
    CreateUDN(G);
    ShortestPath_Floyd(G);
    int op;
    char func[20];
    cin >> op;
    for (int i = 0; i < op; i++)
    {
        cin >> func;
        if (!strcmp(func, "Query"))
        {
            int choice;
            cin >> choice;
            if (choice == 1)
                Query1(G);
            else if (choice == 2)
                Query2(G);
        }
        else if (!strcmp(func, "Modify"))
        {
            Modify(G);
        }
    }
    return 0;
}


/* 功能实现 */
Status CreateUDN(AMGraph &G)
{// 采用邻接矩阵表示法，创建无向网G
    cin >> G.vexnum >> G.arcnum;                    // 输入总顶点数，总边数
    for (int i = 0; i < G.vexnum; i++)              // 依次输入点的信息
        cin >> G.vexs[i].code >> G.vexs[i].name >> G.vexs[i].dec;
    for (int i = 0; i < G.vexnum; i++)
    {                                               // 初始化邻接矩阵，变得权值均置为极大值MaxInt
        for (int j = 0; j < G.vexnum; j++)
            G.arcs[i][j] = MaxInt;
    }
    for (int k = 0; k < G.arcnum; k++)
    {
        int v1, v2;                                 // 一条边依附的两个顶点的编号
        int w;                                      // 权值
        cin >> v1 >> v2 >> w;                       // 输入一条边依附的顶点编号及权值
        int i = v1 - 1;
        int j = v2 - 1;                             // 确定v1、vv2在G中的位置，即顶点数组的下标
        G.arcs[i][j] = w;                           // 边<v1, v2>的权值置为w
        G.arcs[j][i] = G.arcs[i][j];                // 置<v1, v2>的对称边<v2, v1>的权值为w
    }
    return OK;
}

int LocateVexByName(AMGraph G, char *_name)
{// 根据景点名称定位顶点下标
    for (int i = 0; i < G.vexnum; i++)
    {
        if (!(strcmp(G.vexs[i].name, _name)))
            return i;
    }
    return -1;
}

void ShortestPath_Floyd(AMGraph G)
{// 用Floyd算法求无向网G中各对顶点i和j之间的最短路径
    for (int i = 0; i < G.vexnum; i++)              // 初始化
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            dist[i][j] = G.arcs[i][j];              // 初始化为原先每条边的权值
            if (dist[i][j] < MaxInt && i != j)
                path[i][j] = i;                     // 如果i和j之间有边，则将j的前驱置为i
            else
                path[i][j] = -1;                    // 若无边，则将j的前驱置为-1
        }
        
    }
    for (int k = 0; k < G.vexnum; k++)              // Floyd三层循环求解
    {
        for (int i = 0; i < G.vexnum; i++)
        {
            for (int j = 0; j < G.vexnum; j++)
            {                                       
                if ((dist[i][j] > (dist[i][k] + dist[k][j])) && (i != j))
                {                                   // 将权值更新，路径也变成中转点
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];        // 更改j的前驱为k
                }
            }
        }
    }
}

void PrintPath(AMGraph G, int _i, int _j)
{
    if (path[_i][_j] == _i)
    {
        cout << G.vexs[_i].name << "->" << G.vexs[_j].name;
        return;
    }
    PrintPath(G, _i, path[_i][_j]);
    cout << "->" << G.vexs[_j].name;
}

void Query1(AMGraph G)
{// 询问景点信息
    char _name[50];
    cin >> _name;                                   // 景点名
    int i = LocateVexByName(G, _name);
    cout << G.vexs[i].code << " " << G.vexs[i].name
         << " " << G.vexs[i].dec << endl;
}

void Query2(AMGraph G)
{// 路径查询，两景点的最短路径及长度
    char _name1[50], _name2[50];
    cin >> _name1 >> _name2;                        // 输入景点名，直接对应输出最短路径即可
    int _i = LocateVexByName(G, _name1);
    int _j = LocateVexByName(G, _name2);            // 记录两景点下标
    cout << dist[_i][_j] << endl;
    PrintPath(G, _i, _j);
    cout << endl;
}

void Modify(AMGraph &G)
{// 修改景点信息
    char _name[50];
    cin >> _name;
    int i = LocateVexByName(G, _name);              // 定位该景点
    cin >> G.vexs[i].code\
        >> G.vexs[i].name >> G.vexs[i].dec;         // 修改景点信息
}

