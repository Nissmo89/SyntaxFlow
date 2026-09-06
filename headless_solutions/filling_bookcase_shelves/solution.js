/**
 * @param {number[][]} books
 * @param {number} shelfWidth
 * @return {number}
 */
var minHeightShelves = function (books, shelfWidth) {
    const n = books.length;
    const dp = new Array(n + 1).fill(Infinity);
    dp[0] = 0; // empty prefix

    for (let i = 1; i <= n; ++i) {
        let curWidth = 0;
        let curHeight = 0;
        // try shelves that end at book i-1
        for (let j = i; j >= 1; --j) {
            curWidth += books[j - 1][0];
            if (curWidth > shelfWidth) break;
            curHeight = Math.max(curHeight, books[j - 1][1]);
            dp[i] = Math.min(dp[i], dp[j - 1] + curHeight);
        }
    }
    return dp[n];
};
