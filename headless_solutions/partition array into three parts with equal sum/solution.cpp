class Solution {
public:
    bool canThreePartsEqualSum(vector<int>& arr) {
        int total_sum = accumulate(arr.begin(), arr.end(), 0);
        if (total_sum % 3 != 0) return false;
        
        int target = total_sum / 3;
        int current_sum = 0, count = 0;
        
        for (int i = 0; i < arr.size() - 1; ++i) {
            current_sum += arr[i];
            if (current_sum == target) {
                count++;
                current_sum = 0;
                if (count == 2) return true;
            }
        }
        return false;
    }
};
