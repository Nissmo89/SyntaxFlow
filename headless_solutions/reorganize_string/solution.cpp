class Solution {
public:
    string reorganizeString(string s) {
        int n = s.length();
        vector<int> count(26, 0);
        for (char c : s) count[c - 'a']++;
        
        int max_freq = 0, letter = 0;
        for (int i = 0; i < 26; ++i) {
            if (count[i] > max_freq) {
                max_freq = count[i];
                letter = i;
            }
        }
        
        if (max_freq > (n + 1) / 2) return "";
        
        string res(n, ' ');
        int idx = 0;
        // Fill most frequent first
        while (count[letter] > 0) {
            res[idx] = (char)(letter + 'a');
            idx += 2;
            count[letter]--;
        }
        
        // Fill remaining
        for (int i = 0; i < 26; ++i) {
            while (count[i] > 0) {
                if (idx >= n) idx = 1;
                res[idx] = (char)(i + 'a');
                idx += 2;
                count[i]--;
            }
        }
        return res;
    }
};
