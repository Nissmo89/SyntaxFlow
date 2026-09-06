class Solution {
public:
    string originalDigits(string s) {
        vector<int> count(26, 0);
        for (char c : s) count[c - 'a']++;
        
        vector<int> digits(10, 0);
        // Unique identifiers
        digits[0] = count['z' - 'a'];
        digits[2] = count['w' - 'a'];
        digits[4] = count['u' - 'a'];
        digits[6] = count['x' - 'a'];
        digits[8] = count['g' - 'a'];
        
        // Derived identifiers
        digits[1] = count['o' - 'a'] - digits[0] - digits[2] - digits[4];
        digits[3] = count['h' - 'a'] - digits[8];
        digits[5] = count['f' - 'a'] - digits[4];
        digits[7] = count['s' - 'a'] - digits[6];
        digits[9] = count['i' - 'a'] - digits[8] - digits[5] - digits[6];
        
        string res = "";
        for (int i = 0; i < 10; ++i)
            res += string(digits[i], i + '0');
        return res;
    }
};
