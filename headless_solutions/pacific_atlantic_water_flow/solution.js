var pacificAtlantic = function(heights) {
    const m = heights.length, n = heights[0].length;
    const pac = Array.from({length: m}, () => Array(n).fill(false));
    const atl = Array.from({length: m}, () => Array(n).fill(false));
    const dfs = (r, c, visited, prevH) => {
        if (r < 0 || r >= m || c < 0 || c >= n || visited[r][c] || heights[r][c] < prevH) return;
        visited[r][c] = true;
        dfs(r + 1, c, visited, heights[r][c]); dfs(r - 1, c, visited, heights[r][c]);
        dfs(r, c + 1, visited, heights[r][c]); dfs(r, c - 1, visited, heights[r][c]);
    };
    for (let i = 0; i < m; i++) { dfs(i, 0, pac, 0); dfs(i, n - 1, atl, 0); }
    for (let j = 0; j < n; j++) { dfs(0, j, pac, 0); dfs(m - 1, j, atl, 0); }
    const res = [];
    for (let i = 0; i < m; i++)
        for (let j = 0; j < n; j++)
            if (pac[i][j] && atl[i][j]) res.push([i, j]);
    return res;
};
