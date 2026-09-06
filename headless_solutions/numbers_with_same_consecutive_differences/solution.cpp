class Solution {
public:
    vector<int> numsSameConsecDiff(int n, int k) {
        vector<int> cur = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int i = 1; i < n; ++i) {
            vector<int> next;
            for (int x : cur) {
                int y = x % 10;
                if (y + k <= 9) next.push_back(x * 10 + y + k);
                if (k > 0 && y - k >= 0) next.push_back(x * 10 + y - k);
            }
            cur = next;
        }
        return cur;
    }
};
