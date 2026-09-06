class Solution {
public:
    vector<string> uncommonFromSentences(string s1, string s2) {
        unordered_map<string, int> count;
        istringstream iss(s1 + " " + s2);
        string word;
        while (iss >> word) {
            count[word]++;
        }
        vector<string> res;
        for (auto& pair : count) {
            if (pair.second == 1) {
                res.push_back(pair.first);
            }
        }
        return res;
    }
};
