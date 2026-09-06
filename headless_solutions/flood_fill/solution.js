/**
 * @param {number[][]} image
 * @param {number} sr
 * @param {number} sc
 * @param {number} color
 * @return {number[][]}
 */
var floodFill = function(image, sr, sc, color) {
    let originalColor = image[sr][sc];
    dfs(image, sr, sc, originalColor, color);
    return image;
};

function dfs(image, i, j, originalColor, newColor) {
    if (i < 0 || i >= image.length || j < 0 || j >= image[0].length || image[i][j] == newColor || image[i][j] != originalColor) {
        return;
    }
    image[i][j] = newColor;
    dfs(image, i - 1, j, originalColor, newColor);
    dfs(image, i + 1, j, originalColor, newColor);
    dfs(image, i, j - 1, originalColor, newColor);
    dfs(image, i, j + 1, originalColor, newColor);
}
