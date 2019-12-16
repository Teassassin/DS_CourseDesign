/*
 * 单位员工通讯录管理系统（线性表的应用）
 * by TKK
 * Created 2019/12/09
 * Ended 2019/12/09
 */

#include <iostream>
#include <cstring>
#define MAXSIZE 100
using namespace std;

typedef struct Employee                         // 员工结构体
{
    int id;                                     // 员工编号
    char name[MAXSIZE];                         // 员工姓名
    char tel[MAXSIZE];                          // 电话号码
    char email[MAXSIZE];                        // 邮箱
    struct Employee *next;
} *EmployList, Employee;

void CreateList_R(EmployList &L, int n);        // 通讯表的建立
void Query(EmployList L);                       // 查询
void Modify(EmployList L);                      // 修改
void Add(EmployList L);                         // 添加
void Del(EmployList L);                         // 删除
void Print(EmployList L);                       // 打印

int main()
{
    EmployList L;
    char func[20];                              // 暂存函数名
    int n, m;                                   // 要建立表的员工数，操作次数
    cin >> n;
    CreateList_R(L, n);
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        cin >> func;
        if (!strcmp(func, "query"))
            Query(L);
        else if (!strcmp(func, "modify"))
            Modify(L);
        else if (!strcmp(func, "add"))
            Add(L);
        else if (!strcmp(func, "del"))
            Del(L);
        else if(!strcmp(func, "print"))
            Print(L);
    }
    return 0;
}

void CreateList_R(EmployList &L, int n)                               
{// 通讯表的建立
    L = new Employee;
    L->next = NULL;                             // 初始化单链表
    EmployList r = L;
    
    for (int i = 0; i < n; i++)                 // 循环录入n个记录
    {
        EmployList p = new Employee;
        cin >> p->id >> p->name >> p->tel >> p->email;
        r->next = p;
        r = p;
        p->next = NULL;
    }
}

void Query(EmployList L)
{
    char _name[MAXSIZE];                        // 临时存储输入的姓名
    cin >> _name;
    
    EmployList p = L;
    while (p = p->next)                         // 遍历
    {
        if (!strcmp(p->name, _name))
        {
            cout << p->id << ' ' << p->name << ' '\
                 << p->tel << ' ' << p->email << endl;
            return;
        }
    }
    cout << "NOT FOUND!" << endl;               // 没找到输出NOT FOUND!
}

void Modify(EmployList L)
{
    char _name[MAXSIZE];
    cin >> _name;
    char attr[10];                              // 暂存用户输入的属性名
    char info[MAXSIZE];                         // 暂存修改信息
    cin >> attr >> info;                        // 输入

    EmployList p = L;
    while (p = p->next)                         // 遍历
    {
        if (!strcmp(p->name, _name))            // 如果找到此人
        {
            if (!strcmp(attr, "name"))          // 若修改姓名
                strcpy(p->name, info);          // 修改姓名
            else if(!strcmp(attr, "tel"))
                strcpy(p->tel, info);
            else if(!strcmp(attr, "email"))
                strcpy(p->email, info);
        }
    }
}

void Add(EmployList L)
{
    EmployList q = L;
    while (q->next)                             // 循环到底
        q = q->next;

    EmployList p = new Employee;                // 建立新结点
    cin >> p->id >> p->name >> p->tel >> p->email;
    p->next = NULL;                             // 添加完毕，尾结点next指针置空
    q->next = p;                                // 接上
}

void Del(EmployList L)
{
    char _name[MAXSIZE];
    cin >> _name;

    EmployList p = L->next, q = L;              // 需另外一个指针
    while (p)
    {
        if (!strcmp(p->name, _name))            // 找到此人
        {
            q->next = p->next;
            delete p;                           // 删除
            p = NULL;                           // 指针置空
            return;
        }
        p = p->next;
        q = q->next;
    }
}

void Print(EmployList L)
{
    EmployList p = L;
    while (p = p->next)
    {
        cout << p->id << ' ' << p->name << ' '\
                << p->tel << ' ' << p->email << endl;
    }
}