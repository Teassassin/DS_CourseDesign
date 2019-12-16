/*
 * 哈夫曼编码/译码系统（树应用）
 * by TKK
 * Created 2019/12/13 默哀
 * Ended 2019/12/16
 */

#include <iostream>
#include <cstring>
#define MAXSIZE 1001                                            // 字符串最大长度
using namespace std;

/* 结构体与typedef */
typedef struct
{
    int weight;                                                 // 结点的权值
    int parent, lchild, rchild;                                 // 结点的双亲、左孩子、右孩子的下标
    char value;                                                 // 结点的值，即对应的符号
} HTNode, *HuffmanTree;                                         // 动态分配数组存储哈夫曼树

typedef struct Str                                              // 待传送的字符串结构体，方便保存权值等信息
{
    char str_o[MAXSIZE];                                        // 待传送的字符串，original
    char str_s[MAXSIZE];                                        // 待传送的字符串的单个字符的数组，single
    int str_w[MAXSIZE];                                         // 待传送的字符串的权值，weight
} Str;

typedef char **HuffmanCode;                                     // 动态分配数组存储哈夫曼编码表

/* 哈夫曼树基础操作 */
void CreateHuffmanTree(HuffmanTree &HT, Str &str, int n);       // 构造哈夫曼树HT
void Select(HuffmanTree &HT, int k, int &s1, int &s2);          // 选择权值最小的结点
void CreateHuffmanCode(HuffmanTree HT, HuffmanCode &HC, int n); // 求哈夫曼编码，存储在编码表HC中

/* 题目需要的函数 */
void CalculateWeight(Str &str, int n);                          // 计算字符串中单个字符的权值
void EncodeStr(Str &str,HuffmanTree HT, HuffmanCode &HC, int n);// 编码，利用编码表HC根据输入的字符串编码，n是单一字符数组的长度
int DecodeStr(HuffmanTree HT, char *code, int _i, int n);       // 译码，只要我们有相应的哈夫曼树，成功则返回1，可计数，n是单一字符数组的长度

/* 全局变量 */
char decode[MAXSIZE];                                           // 译码
int l_code = 0;                                                 // 要求输出的两个值，哈夫曼编码的最短长度，哈夫曼编码译码的长度


int main()
{
    Str str;                                            // 保存待传送字符串、单一字符数组、权值数组
    HuffmanTree HT;                                     // 哈夫曼树
    HuffmanCode HC;                                     // 哈夫曼编码表
    cin >> str.str_o;                                   // 输入待传送的字符串
    CalculateWeight(str, strlen(str.str_o));            // 计算单一字符以及对应的权值
    int n = strlen(str.str_s);                          // 单一字符数组长度为n
    CreateHuffmanTree(HT, str, n);                      // 构造哈夫曼树
    CreateHuffmanCode(HT, HC, n);                       // 求编码表HC
    EncodeStr(str, HT, HC, n);                          // 编码和译码，以及统计两个长度
    cout << l_code << endl;                             // 输出两个长度
    cout << strlen(decode) << endl;
    return 0;
}


/* 功能实现 */
void CreateHuffmanTree(HuffmanTree &HT, Str &str, int n)
{// 构造哈夫曼树HT，str_w数组用于给结点赋权值
    if (n <= 1)
        return;
    int m = 2 * n - 1;
    HT = new HTNode[m + 1];                             // 0号单元未用，所以需要动态分配m + 1个单元，HT[m]表示根节点
    for (int i = 1; i <= m; ++i)                        // 将1 ~ m号单元中的双亲、左孩子、右孩子的下标都初始化为0，值为'\0'
    {
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
        HT[i].value = '\0';
    }
    for (int i = 1; i <= n; ++i)                        // str_w赋给n个单元中叶子结点的权值，str_s为对应的字符
    {
        HT[i].weight = str.str_w[i - 1];                // i - 1是因为这个算法的i是从1开始的，而计算str_w是从0开始的
        HT[i].value = str.str_s[i - 1];                 
    }
    
    /*---------------------初始化工作结束，下面开始创建哈夫曼树---------------------*/
    int s1, s2;                                         // Select()出来的左右孩子
    for (int i = n + 1; i <= m; ++i)                    // 通过n - 1次的选择、删除、合并来创建哈夫曼树
    {
        Select(HT, i - 1, s1, s2);                      // 选择
        HT[s1].parent = i, HT[s2].parent = i;           // 得到新结点i，从森林中删除s1，s2，将s1和s2的双亲域由0改为i
        HT[i].lchild = s1, HT[i].rchild = s2;           // s1, s2分别作为i的左右孩子
        HT[i].weight = HT[s1].weight + HT[s2].weight;   // i的权值为左右孩子权值之和
    }
}

void Select(HuffmanTree &HT, int k, int &s1, int &s2)
{// 在HT[k](1 ≤ k ≤ i-1)中选择两个其双亲域为0且权值最小的结点，并返回他们在HT中的序号s1和s2
    int i = 1;
    while (HT[i].parent && i <= k)                      // 循环到第一个parent不为0的为止
        i++;
    if (i == k + 1)                                     // 到头也没找到
        return;
    s1 = i++;                                           // s1赋值为i（即第一个parent为0的结点），i后移
    while (HT[i].parent && i <= k)                      // 同样循环到第一个parent不为0的为止
        i++;
    if (i == k + 1)                                     // 到头也没找到
        return;
    s2 = i++;                                           // s2赋值为i（即第二个parent为0的结点），i后移
    if (HT[s1].weight > HT[s2].weight)                  // 如果s1的权值大于s2，则交换他俩的位置
        swap(s1, s2);
    
    for (; i <= k; i++)                                 // 从当前位置开始遍历
    {
        if (!HT[i].parent)                              // 如果遇到parent为0的结点
        {
            if (HT[i].weight < HT[s1].weight)           // 如果比当前的最小值s1小，则i为最小值
            {
                s2 = s1;                                // s1成为次小值赋值给s2
                s1 = i;                                 // i为最小值赋值给s1
            }
            else if (HT[i].weight < HT[s2].weight)      // 这种情况说明比s1大，比s2小，i为次小值
            {
                s2 = i;                                 // i直接赋给s2即可
            }
        }
    }
}

void CreateHuffmanCode(HuffmanTree HT, HuffmanCode &HC, int n)
{// 从叶子到根逆向求每个字符的哈夫曼编码，存储在编码表HC中
    HC = new char *[n + 1];                             // 分配存储n个字符编码的编码表空间
    char *cd = new char[n];                             // 分配临时存放每个字符编码的动态数组空间
    cd[n - 1] = '\0';                                   // 编码结束符
    for (int i = 1; i <= n; ++i)                        // 逐个字符求哈夫曼编码
    {
        int start = n - 1;                              // start开始时指向最后，即编码结束符位置
        int c = i;                                      
        int f = HT[i].parent;                           // f指向结点c的双亲结点
        while (f != 0)
        {
            --start;                                    // 回溯一次start向前指一个位置
            if (HT[f].lchild == c)
                cd[start] = '0';                        // 结点c是f的左孩子，则生成代码0
            else
                cd[start] = '1';                        // 结点c是f的右孩子，则生成代码1
            c = f;
            f = HT[f].parent;                           // 继续向上回溯
        }
        HC[i] = new char[n - start];                    // 为第i个字符编码分配空间
        strcpy(HC[i], &cd[start]);                      // 将求得的编码从临时空间cd复制到HC的当前行中
    }
    delete cd;
}

void CalculateWeight(Str &str, int n)
{// 计算字符串中每个字符的权值（即根据出现频率赋予相应的权值），这里我们会把str_s和str_w算出来
    for (int i = 0; i <= n; i++)                        // single单一字符数组和weight数组初始化为'\0'，0
    {
        str.str_s[i] = '\0';
        str.str_w[i] = 0;
    }
    
    // 计算str_s单一字符数组
    for (int i = 0, j = 0; i < n; i++)                  // 两个循环变量，i，j分别控制str_o和str_s
    {
        bool flag = true;                               // 判断下面的循环是否有重复，true代表没有重复
        str.str_s[j] = str.str_o[i];                    // 不管有没有重复先赋值
        for (int k = 0; k < j; k++)                     // 从0到j，若发现重复的，则j不动，str_s[j]重置为'\0'
        {
            if (str.str_s[j] == str.str_s[k])
            {
                str.str_s[j] = '\0';                    // str_s[j]重置为0
                flag = false;                           // 重复
                break;
            }
        }
        if (flag)                                       // 如果没有重复
            j++;                                        // 继续下一个位置
    }
    
    // 计算str_w
    for (int j = 0; j < n; j++)                         // 以遍历str_s作为外循环，判断字符重复次数，也就是权值
    {
        for (int i = 0; i < n; i++)
        {
            if (str.str_s[j] == str.str_o[i])           // 如果重复
                str.str_w[j]++;                         // 对应的权值加1
        }
    }
}

void EncodeStr(Str &str,HuffmanTree HT, HuffmanCode &HC, int n)
{// 编码，利用编码表HC根据输入的字符串编码，n是单一字符数组的长度
    char code[MAXSIZE];                                 // 也不管了，直接给临时编码最大空间
    for (int i = 0; str.str_o[i] != '\0'; i++)          // 遍历待传送数组
    {
        for (int j = 0; str.str_s[j] != '\0'; j++)      // 遍历单一字符数组
        {
            if (str.str_o[i] == str.str_s[j])           // 找到了对应字符
            {
                // cout << HC[j + 1] << ' ';
                strcpy(code, HC[j + 1]);                // 求得编码，注意编码表HC是从i = 1开始的
                l_code += strlen(code);                 // 累加每一个字符的编码
                DecodeStr(HT, code, i, n);              // 译码
            }
        }
    }
}

int DecodeStr(HuffmanTree HT, char *code, int _i, int n)
{// 译码，只要我们有相应的哈夫曼树，成功则返回1，可计数，_i是译码出的第i个字符
    int m = 2 * n - 1;                                  // 同创建哈夫曼树
    int item = 0;                                       // 记录叶子节点的下标
    for (int i = m, j = 0; HT[i].value == '\0' && code[j] != '\0'; j++)
    {                                                   // 从根节点开始遍历，遇到有value的结点终止，j控制遍历code
        if (code[j] == '0')                             
            i = HT[i].lchild;                           // 如果是0，往左子树走
        else if (code[j] == '1')
            i = HT[i].rchild;                           // 如果是1，往右子树走
        else
            return 0;                                   // 这句是为了防止意外，没实际意义
        item = i;                                       // 记录下当前位置
    }
    decode[_i] = HT[item].value;                        // 叶子结点赋给decode
    return 1;
}