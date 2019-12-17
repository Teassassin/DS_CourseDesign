/*
 * 药店的药品销售统计系统（排序应用）
 * by TKK
 * Created 2019/12/16
 * Ended 2019/12/17
 */

#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

typedef struct                                          // 药品结构体
{
    char num[20];                                       // 药品编号
    char name[50];                                      // 药品名称
    double price;                                       // 药品单价
    int count;                                          // 药品销售数量
    double sale;                                        // 药品销售额sale=price*count
} Drug;

void AddDrug(Drug *drug);                               // 添加药品
void SortByNum(Drug *drug);                             // 根据编号排序（选择排序）
void SortByName(Drug *drug);                            // 根据名称字母排序（冒泡排序）
void SortByPrice(Drug *drug);                           // 根据单价排序（插入排序）
void SortByCount(Drug *drug, int low, int high);        // 根据销售数量排序（快速排序）
void SortBySale(Drug *drug);                            // 根据销售额排序（归并排序）

void PrintSequence(Drug *drug);                         // 打印药品数组
int Partition(Drug *drug, int low, int high);           // 快排分割函数
void MergeSort(Drug *drug, Drug *temp, int s, int t);   // 归并排序
void Merge(Drug *drug, Drug *temp, int s, int m, int t);// 归并排序合并函数

int counts = 0;                                         // 药品总数


int main()
{
    int n;
    cin >> n;
    Drug drug[100];                                 // 药品最多100个
    for (int i = 0; i < n; i++)                     // 添加药品
        AddDrug(drug);

    SortByNum(drug);
    PrintSequence(drug);
    cout << endl;
    SortByName(drug);
    PrintSequence(drug);
    cout << endl;
    SortByPrice(drug);
    PrintSequence(drug);
    cout << endl;
    SortByCount(drug, 0, counts - 1);               // 快速排序
    PrintSequence(drug);
    cout << endl;
    SortBySale(drug);
    PrintSequence(drug);

    return 0;
}


/* 功能实现 */
void AddDrug(Drug *drug)
{// 添加药品
    cin >> drug[counts].num >> drug[counts].name\
        >> drug[counts].price >> drug[counts].count;// 输入药品信息并计算该药品销售额
    drug[counts].sale = drug[counts].price * drug[counts].count;
    counts++;                                       // 药品总数加1
}

void SortByNum(Drug *drug)
{// 根据编号排序（选择排序）
    for (int i = 0; i < counts - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < counts; j++)
        {
            if (strcmp(drug[min].num, drug[j].num) > 0)
                min = j;
        }
        swap(drug[min], drug[i]);
    }
}

void SortByName(Drug *drug)
{// 根据名称字母排序（冒泡排序）
    for (int i = 0; i < counts - 1; i++)
    {
        for (int j = 0; j < counts - i - 1; j++)
        {
            if (strcmp(drug[j].name, drug[j + 1].name) > 0)
                swap(drug[j], drug[j + 1]);
        }
    }
}

void SortByPrice(Drug *drug)
{// 根据单价排序（插入排序）
    for (int i = 1; i < counts; i++)
    {
        Drug key = drug[i];
        int j = i - 1;
        while ((j >= 0) && (key.price < drug[j].price))
        {
            drug[j + 1] = drug[j];
            j--;
        }
        drug[j + 1] = key; 
    }
}

void SortByCount(Drug *drug, int low, int high)
{// 根据销售数量排序（快速排序）
    if (low < high)
    {
        int pivot = Partition(drug, low, high);
        SortByCount(drug, low, pivot - 1);
        SortByCount(drug, pivot + 1, high);
    }
}

void SortBySale(Drug *drug)
{// 根据销售额排序（归并排序）
    Drug temp[100];
    MergeSort(drug, temp, 0, counts - 1);
}

void PrintSequence(Drug *drug)
{// 打印drug数组，我们使用printf，打印小数更方便
    for (int i = 0; i < counts; i++)
    {
        printf("%s\t%s\t", drug[i].num, drug[i].name);
        printf("%.1lf\t%d\t", drug[i].price, drug[i].count);
        printf("%.1lf\n", drug[i].sale);
    }
}

int Partition(Drug *drug, int low, int high)
{// 快排分割函数
    Drug pivot = drug[low];
    while (low < high)
    {
        while (low < high && drug[high].count >= pivot.count)
            --high;
        drug[low] = drug[high];
        while (low < high && drug[low].count <= pivot.count)
            low++;
        drug[high] = drug[low];
    }
    drug[low] = pivot;
    return low;
}

void MergeSort(Drug *drug, Drug *temp, int s, int e)
{// 归并排序
    if (s == e)
        return;
    else
    {
        int m = (s + e) / 2;
        MergeSort(drug, temp, s, m);
        MergeSort(drug, temp, m + 1, e);
        Merge(drug, temp, s, m, e);
    }
}

void Merge(Drug *drug, Drug *temp, int s, int m, int e)
{// 归并排序合并函数
    int i = s;
    int j = m + 1;
    int k = s;
    while (i <= m && j <= e)
    {
        if (drug[i].sale <= drug[j].sale)
            temp[k++] = drug[i++];
        else
            temp[k++] = drug[j++];
    }
    while (i <= m)
        temp[k++] = drug[i++];
    while (j <= e)
        temp[k++] = drug[j++];
    for (int l = s; l <= e; l++)
        drug[l] = temp[l];
}
