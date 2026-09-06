class Solution {
public:
    bool isPossible(vector<int>& nums) {
        unordered_map<int, int> left, end;
        for (int x : nums) left[x]++;
        for (int x : nums) {
            if (left[x] == 0) continue;
            left[x]--;
            if (end[x - 1] > 0) {
                end[x - 1]--;
                end[x]++;
            } else if (left[x + 1] > 0 && left[x + 2] > 0) {
                left[x + 1]--;
                left[x + 2]--;
                end[x + 2]++;
            } else return false;
        }
        return true;
    }
};
