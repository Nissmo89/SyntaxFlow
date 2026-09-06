/**
 * @param {number[][]} img
 * @return {number[][]}
 */
var imageSmoother = function(img) {
    let m = img.length;
    let n = img[0].length;
    let dirs = [[0,1],[0,-1],[1,0],[-1,0],[-1,-1],[1,1],[-1,1],[1,-1]];
    let result = Array(m).fill(0).map(() => Array(n).fill(0));
    
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            let sum = img[i][j];
            let count = 1;
            
            for (let k = 0; k < dirs.length; k++) {
                let x = i + dirs[k][0];
                let y = j + dirs[k][1];
                
                if (x < 0 || x >= m || y < 0 || y >= n) continue;
                
                sum += img[x][y];
                count++;
            }
            
            result[i][j] = Math.floor(sum / count);
        }
    }
    
    return result;
};
