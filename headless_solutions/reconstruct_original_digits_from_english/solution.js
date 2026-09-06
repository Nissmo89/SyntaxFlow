/**
 * @param {string} s
 * @return {string}
 */
var originalDigits = function(s) {
    let count = new Array(26).fill(0);
    for (let char of s) count[char.charCodeAt(0) - 97]++;
    
    let d = new Array(10).fill(0);
    d[0] = count['z'.charCodeAt(0) - 97];
    d[2] = count['w'.charCodeAt(0) - 97];
    d[4] = count['u'.charCodeAt(0) - 97];
    d[6] = count['x'.charCodeAt(0) - 97];
    d[8] = count['g'.charCodeAt(0) - 97];
    d[1] = count['o'.charCodeAt(0) - 97] - d[0] - d[2] - d[4];
    d[3] = count['h'.charCodeAt(0) - 97] - d[8];
    d[5] = count['f'.charCodeAt(0) - 97] - d[4];
    d[7] = count['s'.charCodeAt(0) - 97] - d[6];
    d[9] = count['i'.charCodeAt(0) - 97] - d[8] - d[5] - d[6];
    
    let res = "";
    for (let i = 0; i < 10; i++) res += i.toString().repeat(d[i]);
    return res;
};
