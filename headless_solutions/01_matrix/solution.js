/**
 * @param {number[][]} matrix
 * @return {number[][]}
 */
var updateMatrix = function(matrix) {
    let m = matrix.length;
    let n = matrix[0].length;
    
    let dist = new Array(m).fill(0).map(() => new Array(n).fill(Infinity));
    
    let q = [];
    
    for (let i = 0; i < m; i++) {
        for (let j = 0; j < n; j++) {
            if (matrix[i][j] === 0) {
                dist[i][j] = 0;
                q.push([i, j]);
            }
        }
    }
    
    let dirs = [[-1, 0], [1, 0], [0, -1], [0, 1]];
    
    while (q.length) {
        let cell = q.shift();
        
        for (let dir of dirs) {
            let r = cell[0] + dir[0];
            let c = cell[1] + dir[1];
            
            if (r < 0 || r >= m || c < 0 || c >= n || dist[r][c] <= dist[cell[0]][cell[1]] + 1) continue;
            
            dist[r][c] = dist[cell[0]][cell[1]] + 1;
            q.push([r, c]);
        }
    }
    
    return dist;
};
