class Solution {
public:
    int findMaximumXOR(vector<int>& nums) {
        int max_xor = 0, mask = 0;
        for (int i = 30; i >= 0; --i) {
            mask |= (1 << i);
            unordered_set<int> prefixes;
            for (int num : nums) prefixes.insert(num & mask);
            int candidate = max_xor | (1 << i);
            for (int p : prefixes) {
                if (prefixes.count(p ^ candidate)) {
                    max_xor = candidate;
                    break;
                }
            }
        }
        return max_xor;
    }
};
