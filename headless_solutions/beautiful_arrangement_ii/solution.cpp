class Solution {
public:
    vector<int> constructArray(int n, int k) {
        vector<int> res;
        int left = 1;
        int right = n;
        for (int i = 0; i < n; i++) {
            if (k % 2 == 0) {
                res.push_back(i < k ? left++ : right--);
            } else {
                res.push_back(i < k ? right-- : left++);
            }
            k--;
        }
        return res;
    }
};
