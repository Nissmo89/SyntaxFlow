class Solution {
public:
    int findSubstringInWraproundString(string s) {
        vector<int> count(26, 0);
        int maxLengthCur = 0;
        for (int i = 0; i < s.length(); i++) {
            if (i > 0 && (s[i] - s[i - 1] == 1 || (s[i - 1] == 'z' && s[i] == 'a'))) {
                maxLengthCur++;
            } else {
                maxLengthCur = 1;
            }
            count[s[i] - 'a'] = max(count[s[i] - 'a'], maxLengthCur);
        }
        int sum = 0;
        for (int val : count) sum += val;
        return sum;
    }
};
