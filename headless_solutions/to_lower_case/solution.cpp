class Solution {
public:
    string toLowerCase(string s) {
        string result;
        for (char c : s) {
            if (c >= 'A' && c <= 'Z') {
                result += c + 32;
            } else {
                result += c;
            }
        }
        return result;
    }
};
