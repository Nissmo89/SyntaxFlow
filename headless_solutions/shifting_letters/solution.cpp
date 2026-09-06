class Solution {
public:
    string shiftingLetters(string s, vector<int>& shifts) {
        long long current_shift = 0;
        for (int i = s.length() - 1; i >= 0; --i) {
            current_shift = (current_shift + shifts[i]) % 26;
            int val = s[i] - 'a';
            s[i] = (val + current_shift) % 26 + 'a';
        }
        return s;
    }
};
