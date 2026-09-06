class Solution {
public:
    vector<int> grayCode(int n) {
        int size = 1 << n;
        vector<int> result;
        result.reserve(size);
        for (int i = 0; i < size; ++i) {
            result.push_back(i ^ (i >> 1));
        }
        return result;
    }
};
