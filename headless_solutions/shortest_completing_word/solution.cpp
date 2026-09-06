class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        // Filter out non-letter characters from the license plate and convert it to lowercase
        string plate;
        for (char c : licensePlate) {
            if (isalpha(c)) {
                plate += tolower(c);
            }
        }

        // Count the frequency of each letter in the license plate
        unordered_map<char, int> count;
        for (char c : plate) {
            count[c]++;
        }

        // Initialize the shortest word
        string shortest = "aaaaaaaaaaaaaaaaaaaa";

        // Iterate through the list of words
        for (string word : words) {
            // Count the frequency of each letter in the word
            unordered_map<char, int> wordCount;
            for (char c : word) {
                wordCount[c]++;
            }

            // Check if the word contains all the letters in the license plate
            bool containsAllLetters = true;
            for (auto& pair : count) {
                if (wordCount[pair.first] < pair.second) {
                    containsAllLetters = false;
                    break;
                }
            }

            // If the word contains all the letters and is shorter than the current shortest word, update the shortest word
            if (containsAllLetters && word.length() < shortest.length()) {
                shortest = word;
            }
        }

        return shortest;
    }
};
