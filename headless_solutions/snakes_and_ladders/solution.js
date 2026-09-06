var snakesAndLadders = function(board) {
    const n = board.length;
    const getPos = (s) => {
        let r = Math.floor((s - 1) / n);
        let c = (s - 1) % n;
        if (r % 2 !== 0) c = n - 1 - c;
        return [n - 1 - r, c];
    };
    let q = [[1, 0]], visited = new Set([1]);
    while (q.length) {
        let [curr, dist] = q.shift();
        for (let i = 1; i <= 6; i++) {
            let next = curr + i;
            if (next > n * n) break;
            let [r, c] = getPos(next);
            if (board[r][c] !== -1) next = board[r][c];
            if (next === n * n) return dist + 1;
            if (!visited.has(next)) {
                visited.add(next);
                q.push([next, dist + 1]);
            }
        }
    }
    return -1;
};
