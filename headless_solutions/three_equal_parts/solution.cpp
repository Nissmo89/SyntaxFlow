class Solution {
public:
    vector<int> threeEqualParts(vector<int>& arr) {
        int n = arr.size();
        int count = 0;
        for (int x : arr) if (x == 1) count++;
        if (count == 0) return {0, n - 1};
        if (count % 3 != 0) return {-1, -1};

        int k = count / 3;
        int i1 = -1, i2 = -1, i3 = -1, cur = 0;
        for (int i = 0; i < n; ++i) {
            if (arr[i] == 1) {
                if (cur == 0) i1 = i;
                else if (cur == k) i2 = i;
                else if (cur == 2 * k) i3 = i;
                cur++;
            }
        }

        int len = n - i3;
        for (int i = 0; i < len; ++i) {
            if (arr[i1 + i] != arr[i2 + i] || arr[i2 + i] != arr[i3 + i]) return {-1, -1};
        }
        return {i1 + len - 1, i2 + len};
    }
};
