class Solution {
public:
    bool isSubsequence(string s, string t) {
        int i = 0, j = 0;
        while (j < t.length()) {
            if (i < s.length() && s[i] == t[j]) {
                i++;
            }
            j++;
        }
        return i == s.length();
    }
};
