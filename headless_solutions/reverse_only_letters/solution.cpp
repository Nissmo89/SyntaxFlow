class Solution {
public:
    string reverseOnlyLetters(string s) {
        int i = 0, j = s.length() - 1;
        while (i < j) {
            while (i < j && !isalpha(s[i])) ++i;
            while (i < j && !isalpha(s[j])) --j;
            swap(s[i++], s[j--]);
        }
        return s;
    }
};
