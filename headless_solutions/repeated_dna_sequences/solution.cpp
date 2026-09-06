class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        unordered_set<string> seen, repeated;
        for (int i = 0; i + 9 < s.length(); ++i) {
            string sub = s.substr(i, 10);
            if (!seen.insert(sub).second) {
                repeated.insert(sub);
            }
        }
        return vector<string>(repeated.begin(), repeated.end());
    }
};
