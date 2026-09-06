class Solution {
public:
    string reverseWords(string s) {
        int strIndex = 0;
        while (strIndex < s.length()) {
            // Find the start and end index of the word
            int start = strIndex;
            while (strIndex < s.length() && s[strIndex] != ' ') {
                strIndex++;
            }
            // Reverse the word
            reverse(s.begin() + start, s.begin() + strIndex);
            strIndex++;
        }
        return s;
    }
};
