/**
 * @param {number} n
 * @return {number}
 */
var magicalString = function(n) {
    if (n <= 0) return 0;
    if (n <= 3) return 1;
    let a = new Int32Array(n + 1);
    a[0] = 1; a[1] = 2; a[2] = 2;
    let head = 2, tail = 3, num = 1, count = 1;
    while (tail < n) {
        for (let i = 0; i < a[head]; i++) {
            a[tail] = num;
            if (num === 1 && tail < n) count++;
            tail++;
        }
        num ^= 3;
        head++;
    }
    return count;
};
