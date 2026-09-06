class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        string ans;
        int count = 0;
        for (int i = s.length() - 1; i >= 0; i--) {
            if (s[i] != '-') {
                if (count == k) {
                    ans += '-';
                }
                count = (count + 1) % k;
                ans += toupper(s[i]);
            }
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
