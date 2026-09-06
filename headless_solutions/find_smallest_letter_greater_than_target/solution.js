/**
 * @param {character[]} letters
 * @param {character} target
 * @return {character}
 */
var nextGreatestLetter = function(letters, target) {
    if (target >= letters[letters.length - 1] || target < letters[0]) {
        return letters[0];
    }
    
    let low = 0;
    let high = letters.length - 1;
    while (low <= high) {
        let mid = low + (high - low) / 2;
        if (target >= letters[mid]) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return letters[low];
};
