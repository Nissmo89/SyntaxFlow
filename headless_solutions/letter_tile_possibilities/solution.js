/**
 * @param {string} tiles
 * @return {number}
 */
var numTilePossibilities = function(tiles) {
    const count = new Array(26).fill(0);
    for (let char of tiles) count[char.charCodeAt(0) - 65]++;
    
    const dfs = (arr) => {
        let sum = 0;
        for (let i = 0; i < 26; i++) {
            if (arr[i] > 0) {
                sum++;
                arr[i]--;
                sum += dfs(arr);
                arr[i]++;
            }
        }
        return sum;
    };
    
    return dfs(count);
};
