class Solution {
public:
    vector<int> diStringMatch(string s) {
        vector<int> res;
        int low = 0, high = s.size();
        for (char c : s) {
            if (c == 'I') {
                res.push_back(low++);
            } else {
                res.push_back(high--);
            }
        }
        res.push_back(low); // Append the last value
        return res;
    }
};
