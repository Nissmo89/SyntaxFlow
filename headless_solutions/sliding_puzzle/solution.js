var slidingPuzzle = function(board) {
    const target = "123450";
    let start = board.flat().join('');
    const adj = [[1, 3], [0, 2, 4], [1, 5], [0, 4], [1, 3, 5], [2, 4]];
    let queue = [[start, 0]];
    let visited = new Set([start]);
    
    while (queue.length) {
        let [curr, dist] = queue.shift();
        if (curr === target) return dist;
        let zero = curr.indexOf('0');
        for (let nextIdx of adj[zero]) {
            let arr = curr.split('');
            [arr[zero], arr[nextIdx]] = [arr[nextIdx], arr[zero]];
            let next = arr.join('');
            if (!visited.has(next)) {
                visited.add(next);
                queue.push([next, dist + 1]);
            }
        }
    }
    return -1;
};
