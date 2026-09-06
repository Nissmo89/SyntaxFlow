class Solution {
public:
    int maxTurbulenceSize(vector<int>& arr) {
        int inc = 1, dec = 1, result = 1;
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] > arr[i - 1]) {
                inc = dec + 1;
                dec = 1;
            } else if (arr[i] < arr[i - 1]) {
                dec = inc + 1;
                inc = 1;
            } else {
                inc = 1;
                dec = 1;
            }
            result = max(result, max(inc, dec));
        }
        return result;
    }
};
