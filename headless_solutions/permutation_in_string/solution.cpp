class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s1.length(), m = s2.length();
        if (n > m) return false;
        vector<int> s1_count(26, 0), s2_count(26, 0);
        for (int i = 0; i < n; ++i) {
            s1_count[s1[i] - 'a']++;
            s2_count[s2[i] - 'a']++;
        }
        if (s1_count == s2_count) return true;
        for (int i = n; i < m; ++i) {
            s2_count[s2[i] - 'a']++;
            s2_count[s2[i - n] - 'a']--;
            if (s1_count == s2_count) return true;
        }
        return false;
    }
};
