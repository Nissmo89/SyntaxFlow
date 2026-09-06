var replaceWords = function(dictionary, sentence) {
    const trie = {};
    for (const root of dictionary) {
        let node = trie;
        for (const char of root) {
            if (!node[char]) node[char] = {};
            node = node[char];
        }
        node.isEnd = true;
    }
    
    return sentence.split(' ').map(word => {
        let node = trie;
        let path = "";
        for (const char of word) {
            if (!node[char] || node.isEnd) break;
            path += char;
            node = node[char];
        }
        return node.isEnd ? path : word;
    }).join(' ');
};
