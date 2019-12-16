四、校园导游咨询（图的应用）
=========================

- [题目描述]

------------

设计一个校园导游程序，为来访的客人提供各种信息查询服务。
（1）设计学校的校园平面图（保证各个景点之间连通），以图中顶点表示校内各景点，存放景点名称、代号、简介、编号；以边表示路径，存放路径长度等相关信息； 
（2）提供基本信息的修改功能；
（3）为来访客人提供图中任意景点相关信息的查询；
（4）为来访客人提供景点的问路查询，查询两景点间的最短路径及长度。

- [输入]

--------

第一行输入两个整数n(1≤n≤20) 和 m(0≤m≤n*(n-1)/2)；n 代表学校有 n 个景点，m 代表有 m 条路。
接下来 n 行输入景点信息：每行输入三个字符串str1，str2，str3分别表示景点名称，代号和简介。
接下来m行：每行三个整数u（1≤u≤n），v（1≤v≤n），w(1≤w≤100)；u表示起点，v表示终点，w表示两景点间的距离。
接下来一行输入一个整数op（1≤op≤10）表示操作次数。
接下来op行，每行有一条命令，命令有三种形式。
1.Query 1 str：景点查询，表示查询景点str的相关信息。
2.Query 2 str1 str2：路径查询，表示查询景点str1到达景点str2的最短路径及长度。
3.Modify x str1 str2 str3：表示修改某个的景点信息 ，str1表示景点名称，str2表示景点代号，str3表示景点简介。

- [输出]

--------

Query 1 str：输出一行，str的景点信息。
Query 2 str1 str2：输出两行，第一行输出最短路径长度，第二行输出最短路径（具体格式见输出样例）。