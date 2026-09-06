class Solution {
public:
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        vector<int> bmax(26, 0);
        for (const string& b : words2) {
            vector<int> count(26, 0);
            for (char c : b) count[c - 'a']++;
            for (int i = 0; i < 26; ++i) bmax[i] = max(bmax[i], count[i]);
        }
        
        vector<string> result;
        for (const string& a : words1) {
            vector<int> count(26, 0);
            for (char c : a) count[c - 'a']++;
            bool isUniversal = true;
            for (int i = 0; i < 26; ++i) {
                if (count[i] < bmax[i]) {
                    isUniversal = false;
                    break;
                }
            }
            if (isUniversal) result.push_back(a);
        }
        return result;
    }
};
