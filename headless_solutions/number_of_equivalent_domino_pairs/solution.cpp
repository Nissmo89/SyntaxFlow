class Solution {
public:
    int numEquivDominoPairs(vector<vector<int>>& dominoes) {
        unordered_map<string, int> count;
        int res = 0;
        for (auto& domino : dominoes) {
            string key = to_string(min(domino[0], domino[1])) + to_string(max(domino[0], domino[1]));
            count[key]++;
        }
        for (auto& pair : count) {
            res += pair.second * (pair.second - 1) / 2;
        }
        return res;
    }
};
