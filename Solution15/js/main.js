// // Trie字典树结点
// function Node(word) {
//     Node.prototype = {
//         word: null,                         // 单词本身
//         lines: []                           // 存储包含单词的例句
//     }
//     this.getword = function() {
//         return this.word;
//     }
// }

// var trie = new Array(26)                    // 字典树





// node = new Node()
// node.word = "Successfully"

// console.log(node.getword())

class TrieNode {
    constructor(value) {
        this.value = value;                     // value为单个字符
        this.num = 1;                           
        this.deep = 0;                          // 根节点默认0
        this.son = [];
        this.isEnd = false;
    }
    findNode(value) {                           // 遍历这一层，找相应的字符
        for (let i = 0; i < this.son.length; i++) {
            const node = this.son[i];
            if (node.value == value) {
                return node;
            }
        }
        return null;
    }
}
class Trie {
    constructor() {
        this.root = new TrieNode(null);
        this.size = 1;                          // 一开始的时候只有根结点一个结点
    }
    insert(str) {
        let node = this.root;
        for (let c of str) {
            let snode = node.findNode(c);
            if (snode == null) {
                snode = new TrieNode(c);
                snode.deep = node.deep + 1;
                node.son.push(snode);
            } else {
                snode.num++;                    // 有N个字符串经过它
            }
            node = snode;
        }
        // 如果当前的node已经是一个word，则不需要添加
        if (!node.isEnd) {
            this.size++;
            node.isEnd = true;
        }
    }
    has(str) {
        let node = this.root;
        for (let c of str) {
            const snode = node.findNode(c);
            if (snode) {
                node = snode;
            } else {
                return false;
            }
        }
        return node.isEnd;
    }
}

const nt = new Trie();
nt.insert('name');
nt.insert('word');
nt.insert('happy');
nt.insert('trie');

console.log(nt.has('has'));
console.log(nt.has('trie'));
console.log(nt.has('word'));