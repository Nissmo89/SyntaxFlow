var robotSim = function(commands, obstacles) {
    const obs = new Set(obstacles.map(o => `${o[0]},${o[1]}`));
    const dirs = [[0, 1], [1, 0], [0, -1], [-1, 0]];
    let x = 0, y = 0, d = 0, res = 0;
    for (let c of commands) {
        if (c === -1) d = (d + 1) % 4;
        else if (c === -2) d = (d + 3) % 4;
        else {
            while (c-- > 0 && !obs.has(`${x + dirs[d][0]},${y + dirs[d][1]}`)) {
                x += dirs[d][0]; y += dirs[d][1];
                res = Math.max(res, x * x + y * y);
            }
        }
    }
    return res;
};
