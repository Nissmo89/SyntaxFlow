/**
 * @param {string[]} words
 * @return {string[]}
 */
var findWords = function(words) {
    let line1 = new Set('qwertyuiop');
    let line2 = new Set('asdfghjkl');
    let line3 = new Set('zxcvbnm');
    
    let result = [];
    for (let word of words) {
        let w = new Set(word.toLowerCase());
        if (w.size <= line1.size && [...w].every(c => line1.has(c)) ||
            w.size <= line2.size && [...w].every(c => line2.has(c)) ||
            w.size <= line3.size && [...w].every(c => line3.has(c))) {
            result.push(word);
        }
    }
    return result;
};
