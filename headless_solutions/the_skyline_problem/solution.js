var getSkyline = function(buildings) {
    const events = [];
    for (const [l, r, h] of buildings) {
        events.push([l, -h], [r, h]);
    }
    events.sort((a, b) => a[0] - b[0] || a[1] - b[1]);
    
    const heights = [0];
    const res = [];
    let prev = 0;
    
    for (const [x, h] of events) {
        if (h < 0) heights.push(-h);
        else heights.splice(heights.indexOf(h), 1);
        
        const cur = Math.max(...heights);
        if (cur !== prev) {
            res.push([x, cur]);
            prev = cur;
        }
    }
    return res;
};
