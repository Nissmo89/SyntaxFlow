var openLock = function(deadends, target) {
    const dead = new Set(deadends);
    if (dead.has("0000")) return -1;
    if (target === "0000") return 0;

    let queue = ["0000"];
    const visited = new Set(["0000"]);
    let turns = 0;

    while (queue.length > 0) {
        let nextQueue = [];
        for (const curr of queue) {
            for (let i = 0; i < 4; i++) {
                for (let delta of [-1, 1]) {
                    let digit = (parseInt(curr[i]) + delta + 10) % 10;
                    let next = curr.substring(0, i) + digit + curr.substring(i + 1);
                    if (next === target) return turns + 1;
                    if (!dead.has(next) && !visited.has(next)) {
                        visited.add(next);
                        nextQueue.push(next);
                    }
                }
            }
        }
        queue = nextQueue;
        turns++;
    }
    return -1;
};
