var ladderLength = function(beginWord, endWord, wordList) {
    const set = new Set(wordList);
    if (!set.has(endWord)) return 0;
    
    let queue = [beginWord];
    let depth = 1;
    
    while (queue.length > 0) {
        let size = queue.length;
        for (let i = 0; i < size; i++) {
            let word = queue.shift();
            if (word === endWord) return depth;
            
            for (let j = 0; j < word.length; j++) {
                for (let charCode = 97; charCode <= 122; charCode++) {
                    let next = word.slice(0, j) + String.fromCharCode(charCode) + word.slice(j + 1);
                    if (set.has(next)) {
                        queue.push(next);
                        set.delete(next);
                    }
                }
            }
        }
        depth++;
    }
    return 0;
};
