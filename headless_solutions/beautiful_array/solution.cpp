class Solution {
public:
    vector<int> beautifulArray(int n) {
        vector<int> res = {1};
        while (res.size() < n) {
            vector<int> tmp;
            for (int i : res) if (i * 2 - 1 <= n) tmp.push_back(i * 2 - 1);
            for (int i : res) if (i * 2 <= n) tmp.push_back(i * 2);
            res = tmp;
        }
        return res;
    }
};
