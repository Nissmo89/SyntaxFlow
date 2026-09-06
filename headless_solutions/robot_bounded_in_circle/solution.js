var isRobotBounded = function(instructions) {
    let x = 0, y = 0;
    let dirs = [[0, 1], [1, 0], [0, -1], [-1, 0]];
    let d = 0;
    for (let char of instructions) {
        if (char === 'G') {
            x += dirs[d][0];
            y += dirs[d][1];
        } else if (char === 'L') {
            d = (d + 3) % 4;
        } else {
            d = (d + 1) % 4;
        }
    }
    return (x === 0 && y === 0) || (d !== 0);
};
