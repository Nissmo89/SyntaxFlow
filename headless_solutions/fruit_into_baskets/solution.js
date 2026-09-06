/**
 * @param {number[]} fruits
 * @return {number}
 */
var totalFruit = function(fruits) {
    let counts = new Map();
    let left = 0, maxLen = 0;
    for (let right = 0; right < fruits.length; right++) {
        counts.set(fruits[right], (counts.get(fruits[right]) || 0) + 1);
        while (counts.size > 2) {
            counts.set(fruits[left], counts.get(fruits[left]) - 1);
            if (counts.get(fruits[left]) === 0) counts.delete(fruits[left]);
            left++;
        }
        maxLen = Math.max(maxLen, right - left + 1);
    }
    return maxLen;
};
