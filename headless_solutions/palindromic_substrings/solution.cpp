class Solution {
public:
    int countSubstrings(string s) {
        int count = 0;
        for (int i = 0; i < s.length(); ++i) {
            count += expand(s, i, i);     // Odd length
            count += expand(s, i, i + 1); // Even length
        }
        return count;
    }
private:
    int expand(const string& s, int l, int r) {
        int res = 0;
        while (l >= 0 && r < s.length() && s[l] == s[r]) {
            res++;
            l--;
            r++;
        }
        return res;
    }
};
