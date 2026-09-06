class Solution {
public:
    string findLongestWord(string s, vector<string>& dictionary) {
        string longest = "";
        for (const string& word : dictionary) {
            int i = 0;
            for (char c : s) {
                if (i < word.length() && c == word[i]) i++;
            }
            if (i == word.length()) {
                if (word.length() > longest.length() || (word.length() == longest.length() && word < longest)) {
                    longest = word;
                }
            }
        }
        return longest;
    }
};
