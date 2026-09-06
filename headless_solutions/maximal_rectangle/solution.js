var maximalRectangle = function(matrix) {
    if (!matrix.length) return 0;
    const cols = matrix[0].length;
    let heights = new Array(cols + 1).fill(0);
    let maxArea = 0;
    for (let row of matrix) {
        let stack = [];
        for (let i = 0; i <= cols; i++) {
            if (i < cols) heights[i] = row[i] === '1' ? heights[i] + 1 : 0;
            while (stack.length && heights[stack[stack.length - 1]] >= heights[i]) {
                let h = heights[stack.pop()];
                let w = stack.length === 0 ? i : i - stack[stack.length - 1] - 1;
                maxArea = Math.max(maxArea, h * w);
            }
            stack.push(i);
        }
    }
    return maxArea;
};
