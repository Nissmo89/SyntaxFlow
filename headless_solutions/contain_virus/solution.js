/**
 * @param {number[][]} isInfected
 * @return {number}
 */
var containVirus = function(isInfected) {
    const m = isInfected.length;
    const n = isInfected[0].length;
    const dirs = [[-1,0],[1,0],[0,-1],[0,1]];
    let totalWalls = 0;

    while (true) {
        const visited = Array.from({length: m}, () => Array(n).fill(false));
        const regions = []; // each: {cells:Set, frontier:Set, perimeter:number}

        // 1) discover all infected components
        for (let i = 0; i < m; ++i) {
            for (let j = 0; j < n; ++j) {
                if (isInfected[i][j] === 1 && !visited[i][j]) {
                    const cells = new Set();
                    const frontier = new Set();
                    let perimeter = 0;
                    const stack = [[i,j]];
                    visited[i][j] = true;

                    while (stack.length) {
                        const [x,y] = stack.pop();
                        cells.add(x + ',' + y);
                        for (const [dx,dy] of dirs) {
                            const nx = x + dx, ny = y + dy;
                            if (nx < 0 || nx >= m || ny < 0 || ny >= n) {
                                perimeter++;
                            } else if (isInfected[nx][ny] === 0) {
                                frontier.add(nx + ',' + ny);
                                perimeter++;
                            } else if (isInfected[nx][ny] === 1 && !visited[nx][ny]) {
                                visited[nx][ny] = true;
                                stack.push([nx,ny]);
                            }
                        }
                    }
                    regions.push({cells, frontier, perimeter});
                }
            }
        }

        if (regions.length === 0) break;

        // 2) pick region with largest frontier
        const best = regions.reduce((a,b) => a.frontier.size > b.frontier.size ? a : b);
        totalWalls += best.perimeter;

        // 3) quarantine it
        for (const key of best.cells) {
            const [x,y] = key.split(',').map(Number);
            isInfected[x][y] = 2;
        }

        // 4) spread remaining 1's
        const next = isInfected.map(row => row.slice());
        for (let i = 0; i < m; ++i) {
            for (let j = 0; j < n; ++j) {
                if (isInfected[i][j] === 1) {
                    for (const [dx,dy] of dirs) {
                        const ni = i + dx, nj = j + dy;
                        if (ni >= 0 && ni < m && nj >= 0 && nj < n && isInfected[ni][nj] === 0) {
                            next[ni][nj] = 1;
                        }
                    }
                }
            }
        }
        isInfected = next;
    }
    return totalWalls;
};
