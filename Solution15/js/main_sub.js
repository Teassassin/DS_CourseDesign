/**
 * trie结构参考：https://www.javascriptcn.com/read-75162.html
 * 树节点
 * val: 字母
 * isWord: 是否是最后一个字母
 * children: 孩子节点，每个元素是TreeNode
 * lines: 句子下标数组
 */
class TreeNode {
    constructor(val) {
        this.val = val;
        this.isWord = false;
        this.children = {};
        this.lines = [];
    }
}

/**
 * Initialize your data structure here
 * 初始化字典树，只有根元素，也是一个节点
 * 同时根元素没有val，故TreeNode不需要传参数
 * 暂时没有子元素，所以它的children属性暂时是：{}空对象
 * 不包含任何字母，所以isWord仍是默认的false
 */
var Trie = function () {
    this.root = new TreeNode();
};

/**
 * Insert a word into the trie.
 * @param {string} word
 * @param {array} lines
 * @return {void}
 */
Trie.prototype.insert = function (word, lines) {
    let curNode = this.root;
    let arr = word.split('');
    for (let i = 0; i < arr.length; i++) {
        let isHasChildNode = curNode.children[arr[i]];
        // 没有子节点的话，就要创建一个以当前字符为val的子节点
        if (!isHasChildNode) {
            curNode.children[arr[i]] = new TreeNode(arr[i]);
        }
        curNode = curNode.children[arr[i]];
        // 遍历到最后一个字符所对应的节点，将这个节点的isWord属性设为true
        if (i === arr.length - 1) {
            curNode.isWord = true;
            curNode.lines = lines;
        }
    }
}

/**
 * Returns if the word is in the trie and get its lines
 * @param {string} word
 * @param {array} lines=[]
 * @return {array}
 */
Trie.prototype.search = function (word) {
    // flag判断是否找到单词或词干
    let flag = false;
    // lines记录例句下标
    let lines = []
    let curNode = this.root;
    let arr = word.split('');
    for (let i = 0; i < arr.length; i++) {
        if (!curNode.children[arr[i]]) {
            return false;
        }
        curNode = curNode.children[arr[i]];
        if (curNode.isWord == true) {
            // 查询过程发现词干
            // 暂时记录下lines，如果最后找不到对应单词，就使用词干的例句
            lines = curNode.lines;
        }
        // 搜索到最后一个字符，根据isWord属性判断是否曾经存过这个单词
        if (i === arr.length - 1) {
            if (curNode.isWord === true) {
                // 记录该词的例句
                lines = curNode.lines;
                break;
            }
        }
    }
    return lines;
}

/**
 * Returns if there is any word in the trie that starts with the given prefix
 * @param {string} prefix
 * @return {boolean}
 */
Trie.prototype.startsWith = function (prefix) {
    let curNode = this.root;
    let arr = prefix.split('');
    for (let i = 0; i < arr.length; i++) {
        // 凡是查找的单词的中间某个字符，没有找到节点的，返回false
        if (!curNode.children[arr[i]]) {
            return false;
        }
        curNode = curNode.children[arr[i]];
    }
    return true;
};

var trie = new Trie();
// var count = 0;              // 显示例句的计数
var words = "";             // 单词框要显示的所有单词
var line_index = [];        // 添加句子时记录句子下标，防止重复

$(document).ready(function () {
    /**
     * 插入索引中的所有单词
     */
    for (let i = 0; i < index.length; i++) {
        trie.insert(index[i][0], index[i][1]);
        // console.log(index[i]);
    }
    /**
     * 响应点击和回车
     */
    $("#bt").click(function () {
        // 初始化例句数和文本
        // count = 0;
        words = "";
        line_index = [];
        $("#examples").text("");
        // 支持大写
        let strval = document.getElementById("inputbox").value.toLowerCase();
        // 分割字符串，循环每个单词
        strval.split(" ").forEach(str => {
            search_sub(str);
        });
        if ($("#examples").text().length <= 0) {
            $("#examples").text("Sorry, word(s) not in lib yet!");
        }
    });
    $("#inputbox").keyup(function (event) {
        if (event.keyCode == 13) {
            $("#bt").click();
        }
    });
    /**
     * 接收主界面传来的参数
     */
    let strval = "";
    // 获取url中?后面的字符串，注意要使用decodeURI，否则空格会变成%20
    let url = decodeURI(window.location.search);
    if (url.indexOf("?") != -1) {
        strval = url.substr(url.indexOf("=") + 1);
    }
    if (strval != "") { // 如果接收值不为空就搜索
        $("#inputbox").val(strval);
        $("#bt").click();
    }
});

/**
 * 搜索
 */
function search_sub(str) {
    if (str.length < 2)     // 对于一个字母的情况不予接受
        return;
    let lines = [];
    // 如果搜索到0个以上例句
    if ((lines = trie.search(str)).length > 0) {
        words += str + " "; // word单词框
        $(".word").text(words);
        $("#examples").append("<p style='color: #0078D4; font-size: larger;'>" + str);
        showExamples(lines);// 显示例句
    }
}

/**
 * 显示例句
 */
function showExamples(lines = []) {
    for (let i = 0; i < lines.length; i++) {
        // // 防止例句重复显示，学到indexOf()查找元素，找不到返回-1
        // if (line_index.indexOf(lines[i]) == -1) {
        //     line_index.push(lines[i]);      // 不重复则记录在line_index中
        // } else {
        //     continue;                       // 重复则不显示
        // }

        // 添加例句
        $("#examples").append((i + 1).toString() + ". " + lines_all[lines[i]] + "<br>");
        // count++;
    }
}