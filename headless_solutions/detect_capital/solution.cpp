class Solution {
public:
    bool detectCapitalUse(string word) {
        int count = 0;
        for (char c : word) {
            if (isupper(c)) {
                count++;
            }
        }
        return count == 0 || count == word.size() || (count == 1 && isupper(word[0]));
    }
};
