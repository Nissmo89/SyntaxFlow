class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        vector<int> cuts(n);
        for (int i = 0; i < n; ++i) cuts[i] = i;

        for (int i = 0; i < n; ++i) {
            // Odd length
            for (int l = i, r = i; l >= 0 && r < n && s[l] == s[r]; --l, ++r) {
                cuts[r] = min(cuts[r], (l == 0 ? 0 : cuts[l - 1] + 1));
            }
            // Even length
            for (int l = i, r = i + 1; l >= 0 && r < n && s[l] == s[r]; --l, ++r) {
                cuts[r] = min(cuts[r], (l == 0 ? 0 : cuts[l - 1] + 1));
            }
        }
        return cuts[n - 1];
    }
};
