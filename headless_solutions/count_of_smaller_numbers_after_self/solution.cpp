class Solution {
    void merge(vector<int>& indices, int left, int mid, int right, vector<int>& counts, const vector<int>& nums) {
        vector<int> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        while (i <= mid && j <= right) {
            if (nums[indices[i]] > nums[indices[j]]) {
                counts[indices[i]] += (right - j + 1);
                temp[k++] = indices[i++];
            } else {
                temp[k++] = indices[j++];
            }
        }
        while (i <= mid) temp[k++] = indices[i++];
        while (j <= right) temp[k++] = indices[j++];
        for (int p = 0; p < temp.size(); ++p) indices[left + p] = temp[p];
    }

    void mergeSort(vector<int>& indices, int left, int right, vector<int>& counts, const vector<int>& nums) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(indices, left, mid, counts, nums);
        mergeSort(indices, mid + 1, right, counts, nums);
        merge(indices, left, mid, right, counts, nums);
    }

public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        vector<int> counts(n, 0), indices(n);
        for (int i = 0; i < n; ++i) indices[i] = i;
        mergeSort(indices, 0, n - 1, counts, nums);
        return counts;
    }
};
