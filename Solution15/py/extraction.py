'''
处理语料
正则表达式分割语句
TKK 2019/12/21
'''

import re


def extraction():
    # 保存最后的句子列表，等待写入文件
    result = []
    # 计数器，每50个句子写入一次文件
    counts = 0
    # 写入文件的后缀名
    postfix = ''
    print('===============doing==============')

    for i in range(1, 11):
        # 打开resource1~10
        path_read = '..\\pre_src\\resource' + str(i) + '.txt'
        file_read = open(path_read, mode='r', encoding='utf-8')
        # passage保存整个文件的字符串
        passage = file_read.readlines()
        # 利用正则表达式分割句子
        # pattern = '.*?(?<!\bMr\b)[\.\?](?=\s+(?:[A-Z]|$))'
        # pattern = '[\.\?"]( *)(\n*)'
        # group1: 用于筛选首行缩进的空格
        # 然后是|，或
        # group2: .或?或: 非数字 空格零个或多个 双引号零个或多个 换行零个或多个
        pattern = '(  +)|([\.\?:](\D)( *)("*)(\n*))'
        # 对遍历每一段
        for i in passage:
            paragraph = list(re.split(pattern, i))
            for j in paragraph:
                if (j != '') and (j != ' ') and (j != '\n') and (j != None):
                    # 排除没用的字符
                    if len(j) > 30:
                        # 排除长度小于30的句子，同时筛掉了无意义的字符和单词
                        result.append(j)
                        counts += 1
                        if (counts % 50 == 0) and (counts != 0):
                            # 够50个则写入文件
                            postfix = str(int(counts / 50)) + '.txt'
                            print("Writing file: {} ......".format('result' + postfix))
                            path_write = '..\\src\\result' + postfix
                            file_write = open(path_write, mode='w', encoding='utf-8')
                            for k in result:
                                # 逐行写入
                                # 处理句末的.
                                k = k.rstrip('.')
                                file_write.write(k + '\n')
                            # result重置
                            result = []
    # 最后将不够50个的写入一次
    if result != []:
        postfix = str(int(counts / 50 + 1)) + '.txt'
        print("Writing file: {} ......".format('result' + postfix))
        path_write = '..\\src\\result' + postfix
        file_write = open(path_write, mode='w', encoding='utf-8')
        for k in result:
            k = k.rstrip('.')
            file_write.write(k + '\n')

    print("{} lines writen.".format(counts))
    print('===============done===============')
    file_read.close()
    file_write.close()
