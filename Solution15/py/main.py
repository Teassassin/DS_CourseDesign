'''
提取句子
写入句子数组文件.js
写入索引文件.js
TKK 2019/12/22
end 2019/12/26
'''

import webbrowser as web
import re
from collections import defaultdict

'''
nltk是自然语言处理库
stemmer是词干处理
porter算法是最快的一个
然而import这个库还是明显拖慢了运行速度
'''
import nltk.stem.porter as pt
stemmer = pt.PorterStemmer().stem

# import sys
# sys.path.append("..")
import extraction
# import trie


def hasDigit(str_tmp):
    '''
    判断字符串是否包含数字
    自认为相比isalpha()可以降低时间复杂度
    '''
    for i in str_tmp:
        if i.isdigit():
            return True
    return False

def main():
    '''
    主函数
    1. 提取句子
    2. 生成句子数组文件
    3. 生成索引，写入index.js文件
    '''
    # 计数器
    counts_files = 1
    counts_words = 0
    counts_involve = 0
    counts_lines = 0
    # 句子数组初始化为空
    lines_raw = []
    lines = []
    # 所有单词的索引，字典类型
    index = defaultdict(set)
    # 先打开两个js文件等待写入
    file_write_lines = open("..\\js\\lines_all.js", mode='w', encoding='utf-8')
    file_write_index = open("..\\js\\index.js", mode='w', encoding='utf-8')

    # 1. 提取句子
    extraction.extraction()
    while True:
        try:
            # 逐个读取语料
            path_read = "..\\src\\result" + str(counts_files) + ".txt"
            file_read = open(path_read, mode='r', encoding='utf-8')
            lines_raw.append(file_read.readlines())
            counts_files += 1
        except:
            file_read.close()
            break
    # 处理句末的换行符
    for i in lines_raw:
        for j in i:
            # 待解决：疑问句怎么办
            j = j.rstrip('\n') + '.'
            lines.append(j)
    
    # 2. 生成句子数组文件
    file_write_lines.write("var lines_all = [];\n")
    for item in lines:
        file_write_lines.write('lines_all[{0}] = "{1}";\n'.format(counts_lines, item))
        counts_lines += 1

    # 3. 生成索引，写入index.js文件
    # 1) 生成索引
    # 查得三引号可以包含所有单引号和双引号
    pattern = """,?;?"?,?'? "?'?\.?|-"""
    # enumerate是真的好用，i是下标，item是对应的元素
    for i, item in enumerate(lines):
        for word in re.split(pattern, item):
            # 如果不包含数字并且不是空格
            if (word != "") and (not hasDigit(word)) and (not word.isspace()):
                # print(word, end=' ')
                word = word.lower()
                # 前面句子加上了. 这里还要去掉
                word = word.rstrip('.')
                index[word].add(i)
                index[stemmer(word)].add(i)
                counts_involve += 1
    # 2) 写入index.js
    file_write_index.write("var index = [];\n")
    for key, value in index.items():
        file_write_index.write('index[{0}] = ["{1}", {2}];\n'.format(counts_words, key, list(value)))
        counts_words += 1

    # 收尾
    print("{0} words inserted. {1} involved.".format(counts_words, counts_involve))
    file_write_lines.close()
    file_write_index.close()

    # 4. 所有操作完成，打开页面
    web.open("..\\s15.html")


if __name__ == '__main__':
    main()
