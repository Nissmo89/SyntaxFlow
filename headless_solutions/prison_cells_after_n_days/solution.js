/**
 * @param {number[]} cells
 * @param {number} n
 * @return {number[]}
 */
var prisonAfterNDays = function(cells, n) {
    const seen = new Map();
    while (n > 0) {
        seen.set(cells.join(','), n--);
        let next = new Array(8).fill(0);
        for (let i = 1; i < 7; i++) {
            next[i] = cells[i - 1] === cells[i + 1] ? 1 : 0;
        }
        cells = next;
        if (seen.has(cells.join(','))) {
            n %= (seen.get(cells.join(',')) - n);
        }
    }
    return cells;
};
