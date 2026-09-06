/**
 * @param {number[][]} image
 * @return {number[][]}
 */
var flipAndInvertImage = function(image) {
    let n = image.length;
    for (let i = 0; i < n; i++) {
        let left = 0;
        let right = n - 1;
        while (left <= right) {
            if (image[i][left] == image[i][right]) {
                image[i][left] = 1 - image[i][left];
                image[i][right] = 1 - image[i][left];
            }
            left++;
            right--;
        }
    }
    return image;
};
