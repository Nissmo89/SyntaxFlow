var threeEqualParts = function(arr) {
    const n = arr.length;
    const ones = arr.map((val, idx) => val === 1 ? idx : -1).filter(idx => idx !== -1);
    if (ones.length === 0) return [0, n - 1];
    if (ones.length % 3 !== 0) return [-1, -1];

    const k = ones.length / 3;
    const i1 = ones[0], i2 = ones[k], i3 = ones[2 * k];
    const len = n - i3;

    for (let i = 0; i < len; i++) {
        if (arr[i1 + i] !== arr[i2 + i] || arr[i2 + i] !== arr[i3 + i]) return [-1, -1];
    }
    return [i1 + len - 1, i2 + len];
};
