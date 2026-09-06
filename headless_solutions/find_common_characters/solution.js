/**
 * @param {string[]} words
 * @return {string[]}
 */
var commonChars = function(words) {
    let freq = new Array(26).fill(0);
    // Initialize freq with the first word
    for (let c of words[0]) {
        freq[c.charCodeAt(0) - 'a'.charCodeAt(0)]++;
    }
    
    // Update freq for each subsequent word
    for (let word of words.slice(1)) {
        let temp_freq = new Array(26).fill(0);
        for (let c of word) {
            temp_freq[c.charCodeAt(0) - 'a'.charCodeAt(0)]++;
        }
        for (let i = 0; i < 26; i++) {
            freq[i] = Math.min(freq[i], temp_freq[i]);
        }
    }
    
    // Construct the result
    let result = [];
    for (let i = 0; i < 26; i++) {
        while (freq[i] > 0) {
            result.push(String.fromCharCode('a'.charCodeAt(0) + i));
            freq[i]--;
        }
    }
    return result;
};
