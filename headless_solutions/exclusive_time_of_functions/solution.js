/**
 * @param {number} n
 * @param {string[]} logs
 * @return {number[]}
 */
var exclusiveTime = function (n, logs) {
    const ans = new Array(n).fill(0);
    const stack = [];               // holds function ids
    let prev = 0;

    for (const log of logs) {
        const [idStr, type, timeStr] = log.split(':');
        const id = Number(idStr);
        const time = Number(timeStr);

        if (type === 'start') {
            if (stack.length) {
                ans[stack[stack.length - 1]] += time - prev;
            }
            stack.push(id);
            prev = time;
        } else { // 'end'
            const top = stack.pop();
            ans[top] += time - prev + 1;
            prev = time + 1;
        }
    }
    return ans;
};
