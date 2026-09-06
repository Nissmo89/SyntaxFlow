var maxEnvelopes = function(envelopes) {
    envelopes.sort((a, b) => a[0] === b[0] ? b[1] - a[1] : a[0] - b[0]);
    let tails = [];
    for (let [w, h] of envelopes) {
        let left = 0, right = tails.length;
        while (left < right) {
            let mid = Math.floor((left + right) / 2);
            if (tails[mid] < h) left = mid + 1;
            else right = mid;
        }
        if (left === tails.length) tails.push(h);
        else tails[left] = h;
    }
    return tails.length;
};
