var minWindow = function(s, t) {
    let map = new Array(128).fill(0);
    for (let char of t) map[char.charCodeAt(0)]++;
    let counter = t.length, begin = 0, end = 0, d = Infinity, head = 0;
    while (end < s.length) {
        if (map[s.charCodeAt(end++)]-- > 0) counter--;
        while (counter === 0) {
            if (end - begin < d) d = end - (head = begin);
            if (map[s.charCodeAt(begin++)]++ === 0) counter++;
        }
    }
    return d === Infinity ? "" : s.substring(head, head + d);
};
