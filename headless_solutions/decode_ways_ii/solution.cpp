class Solution {
public:
    int numDecodings(string s) {
        long long MOD = 1e9 + 7;
        long long prev2 = 1, prev1 = 0;
        
        // Initial state: empty string
        if (s[0] == '*') prev1 = 9;
        else if (s[0] != '0') prev1 = 1;
        
        for (int i = 1; i < s.length(); ++i) {
            long long curr = 0;
            // Single digit
            if (s[i] == '*') curr = (prev1 * 9) % MOD;
            else if (s[i] != '0') curr = prev1;
            
            // Two digits
            string sub = s.substr(i - 1, 2);
            if (sub == "**") curr = (curr + prev2 * 15) % MOD;
            else if (s[i-1] == '*') curr = (curr + prev2 * (s[i] <= '6' ? 2 : 1)) % MOD;
            else if (s[i] == '*') curr = (curr + prev2 * (s[i-1] == '1' ? 9 : (s[i-1] == '2' ? 6 : 0))) % MOD;
            else {
                int val = stoi(sub);
                if (val >= 10 && val <= 26) curr = (curr + prev2) % MOD;
            }
            prev2 = prev1;
            prev1 = curr;
        }
        return (int)prev1;
    }
};
