class Solution {
public:
    int longestPalindrome(string s) {
        unordered_map<char, int> frequencyMap;
        for (char c : s) {
            frequencyMap[c]++;
        }
        int res = 0;
        bool hasOddFrequency = false;
        for (auto& pair : frequencyMap) {
            if (pair.second % 2 == 0) {
                res += pair.second;
            } else {
                res += pair.second - 1;
                hasOddFrequency = true;
            }
        }
        if (hasOddFrequency) {
            return res + 1;
        } else {
            return res;
        }
    }
};
