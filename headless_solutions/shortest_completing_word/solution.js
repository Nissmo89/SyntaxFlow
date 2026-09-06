/**
 * @param {string} licensePlate
 * @param {string[]} words
 * @return {string}
 */
var shortestCompletingWord = function(licensePlate, words) {
    // Filter out non-letter characters from the license plate and convert it to lowercase
    let plate = '';
    for (let c of licensePlate) {
        if (c.match(/[a-z]/i)) {
            plate += c.toLowerCase();
        }
    }

    // Count the frequency of each letter in the license plate
    let count = {};
    for (let c of plate) {
        count[c] = (count[c] || 0) + 1;
    }

    // Initialize the shortest word
    let shortest = 'aaaaaaaaaaaaaaaaaaaa';

    // Iterate through the list of words
    for (let word of words) {
        // Count the frequency of each letter in the word
        let wordCount = {};
        for (let c of word) {
            wordCount[c] = (wordCount[c] || 0) + 1;
        }

        // Check if the word contains all the letters in the license plate
        let containsAllLetters = true;
        for (let c in count) {
            if (wordCount[c] < count[c]) {
                containsAllLetters = false;
                break;
            }
        }

        // If the word contains all the letters and is shorter than the current shortest word, update the shortest word
        if (containsAllLetters && word.length < shortest.length) {
            shortest = word;
        }
    }

    return shortest;
};
