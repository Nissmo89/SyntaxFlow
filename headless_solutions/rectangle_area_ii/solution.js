/**
 * @param {number[][]} rectangles
 * @return {number}
 */
var rectangleArea = function(rectangles) {
    let events = [];
    for (let [x1, y1, x2, y2] of rectangles) {
        events.push([x1, 1, y1, y2], [x2, -1, y1, y2]);
    }
    events.sort((a, b) => a[0] - b[0]);
    
    let active = [], area = 0n, prevX = events[0][0], MOD = 1000000007n;
    
    for (let [x, type, y1, y2] of events) {
        let width = BigInt(x - prevX);
        if (width > 0n) {
            active.sort((a, b) => a[0] - b[0]);
            let height = 0n, lastY = -1n;
            for (let [ys, ye] of active) {
                let start = BigInt(ys), end = BigInt(ye);
                lastY = lastY > start ? lastY : start;
                height += (end > lastY ? end - lastY : 0n);
                lastY = lastY > end ? lastY : end;
            }
            area = (area + width * height) % MOD;
        }
        if (type === 1) active.push([y1, y2]);
        else {
            let idx = active.findIndex(i => i[0] === y1 && i[1] === y2);
            active.splice(idx, 1);
        }
        prevX = x;
    }
    return Number(area);
};
