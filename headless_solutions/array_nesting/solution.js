/**
 * @param {number[]} nums
 * @return {number}
 */
var arrayNesting = function(nums) {
    const n = nums.length;
    const visited = new Array(n).fill(false);
    let max_length = 0;
    
    for (let i = 0; i < n; i++) {
        if (!visited[i]) {
            let length = 0;
            let current = i;
            while (!visited[current]) {
                visited[current] = true;
                length++;
                current = nums[current];
            }
            max_length = Math.max(max_length, length);
        }
    }
    
    return max_length;
};
