class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n, -1);
        vector<int> st; // Monotonic stack storing indices
        for (int i = 0; i < 2 * n; ++i) {
            while (!st.empty() && nums[st.back()] < nums[i % n]) {
                res[st.back()] = nums[i % n];
                st.pop_back();
            }
            if (i < n) st.push_back(i);
        }
        return res;
    }
};
