class Solution {
public:
    int longestMountain(vector<int>& arr) {
        int n = arr.size();
        int maxLen = 0;
        for (int i = 1; i < n - 1; ++i) {
            // Check if i is a peak
            if (arr[i] > arr[i - 1] && arr[i] > arr[i + 1]) {
                int left = i - 1, right = i + 1;
                while (left > 0 && arr[left - 1] < arr[left]) left--;
                while (right < n - 1 && arr[right] > arr[right + 1]) right++;
                maxLen = max(maxLen, right - left + 1);
                i = right; // Skip to end of this mountain
            }
        }
        return maxLen;
    }
};
