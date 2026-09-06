var multiply = function(num1, num2) {
    if (num1 === "0" || num2 === "0") return "0";
    const m = num1.length, n = num2.length;
    const pos = new Array(m + n).fill(0);
    for (let i = m - 1; i >= 0; i--) {
        for (let j = n - 1; j >= 0; j--) {
            const mul = (num1[i] - '0') * (num2[j] - '0');
            const p1 = i + j, p2 = i + j + 1;
            const sum = mul + pos[p2];
            pos[p2] = sum % 10;
            pos[p1] += Math.floor(sum / 10);
        }
    }
    let res = pos.join('').replace(/^0+/, '');
    return res === "" ? "0" : res;
};
