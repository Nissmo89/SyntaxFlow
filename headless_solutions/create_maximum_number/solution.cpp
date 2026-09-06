class Solution {
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        vector<int> best;
        int start = max(0, k - n);
        int end   = min(k, m);
        for (int i = start; i <= end; ++i) {
            vector<int> sub1 = maxSubsequence(nums1, i);
            vector<int> sub2 = maxSubsequence(nums2, k - i);
            vector<int> merged = merge(sub1, sub2);
            if (greater(merged, 0, best, 0)) best = merged;
        }
        return best;
    }

private:
    // maximal subsequence of length k from a single array
    vector<int> maxSubsequence(vector<int>& nums, int k) {
        vector<int> stack;
        int drop = (int)nums.size() - k;
        for (int num : nums) {
            while (drop > 0 && !stack.empty() && stack.back() < num) {
                stack.pop_back();
                --drop;
            }
            stack.push_back(num);
        }
        stack.resize(k);
        return stack;
    }

    // merge two subsequences greedily
    vector<int> merge(vector<int>& a, vector<int>& b) {
        vector<int> res;
        int i = 0, j = 0;
        while (i < a.size() || j < b.size()) {
            if (greater(a, i, b, j)) res.push_back(a[i++]);
            else                     res.push_back(b[j++]);
        }
        return res;
    }

    // lexicographic comparison of suffixes starting at i and j
    bool greater(vector<int>& a, int i, vector<int>& b, int j) {
        while (i < a.size() && j < b.size() && a[i] == b[j]) {
            ++i; ++j;
        }
        return j == b.size() || (i < a.size() && a[i] > b[j]);
    }
};
