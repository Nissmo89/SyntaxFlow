/**
 * @param {number[][]} matrix
 * @param {number} k
 * @return {number}
 */
var maxSumSubmatrix = function(matrix, k) {
    const m = matrix.length;
    const n = matrix[0].length;
    let ans = -Infinity;

    const colIsOuter = m > n;
    const R = colIsOuter ? n : m;
    const C = colIsOuter ? m : n;

    for (let l = 0; l < R; ++l) {
        const rowSums = new Array(C).fill(0);
        for (let r = l; r < R; ++r) {
            for (let i = 0; i < C; ++i) {
                rowSums[i] += colIsOuter ? matrix[i][r] : matrix[r][i];
            }

            // Kadane's algorithm check
            let curKadane = rowSums[0];
            let maxKadane = rowSums[0];
            for (let i = 1; i < C; ++i) {
                curKadane = Math.max(rowSums[i], curKadane + rowSums[i]);
                maxKadane = Math.max(maxKadane, curKadane);
            }

            if (maxKadane <= k) {
                ans = Math.max(ans, maxKadane);
                if (ans === k) return k;
                continue;
            }

            // Binary search in a sorted array
            const prefSet = [0];
            let pref = 0;

            for (let i = 0; i < C; ++i) {
                pref += rowSums[i];
                const target = pref - k;

                // Binary search for smallest element >= target
                let low = 0, high = prefSet.length - 1;
                let candidateIdx = -1;
                while (low <= high) {
                    const mid = Math.floor((low + high) / 2);
                    if (prefSet[mid] >= target) {
                        candidateIdx = mid;
                        high = mid - 1;
                    } else {
                        low = mid + 1;
                    }
                }

                if (candidateIdx !== -1) {
                    ans = Math.max(ans, pref - prefSet[candidateIdx]);
                    if (ans === k) return k;
                }

                // Insert pref maintaining sorted order
                low = 0; high = prefSet.length - 1;
                let insertIdx = prefSet.length;
                while (low <= high) {
                    const mid = Math.floor((low + high) / 2);
                    if (prefSet[mid] >= pref) {
                        insertIdx = mid;
                        high = mid - 1;
                    } else {
                        low = mid + 1;
                    }
                }
                prefSet.splice(insertIdx, 0, pref);
            }
        }
    }

    return ans;
};
