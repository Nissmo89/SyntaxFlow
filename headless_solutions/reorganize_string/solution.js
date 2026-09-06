var reorganizeString = function(s) {
    const n = s.length;
    const count = new Array(26).fill(0);
    for (let char of s) count[char.charCodeAt(0) - 97]++;
    
    let maxFreq = 0, letter = 0;
    for (let i = 0; i < 26; i++) {
        if (count[i] > maxFreq) {
            maxFreq = count[i];
            letter = i;
        }
    }
    
    if (maxFreq > Math.floor((n + 1) / 2)) return "";
    
    const res = new Array(n);
    let idx = 0;
    while (count[letter] > 0) {
        res[idx] = String.fromCharCode(letter + 97);
        idx += 2;
        count[letter]--;
    }
    
    for (let i = 0; i < 26; i++) {
        while (count[i] > 0) {
            if (idx >= n) idx = 1;
            res[idx] = String.fromCharCode(i + 97);
            idx += 2;
            count[i]--;
        }
    }
    return res.join('');
};
