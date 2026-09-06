/**
 * @param {number} numRows
 * @return {number[][]}
 */
var generate = function(numRows) {
    let r = [];
    
    for (let i = 0; i < numRows; i++) {
        let row = new Array(i + 1).fill(1);
        
        for (let j = 1; j < i; j++)
            row[j] = r[i - 1][j - 1] + r[i - 1][j];
        
        r.push(row);
    }
    
    return r;
};
