class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int cand1 = 0, cand2 = 0, count1 = 0, count2 = 0;
        for (int n : nums) {
            if (n == cand1) count1++;
            else if (n == cand2) count2++;
            else if (count1 == 0) { cand1 = n; count1 = 1; }
            else if (count2 == 0) { cand2 = n; count2 = 1; }
            else { count1--; count2--; }
        }
        count1 = count2 = 0;
        for (int n : nums) {
            if (n == cand1) count1++;
            else if (n == cand2) count2++;
        }
        vector<int> res;
        if (count1 > nums.size() / 3) res.push_back(cand1);
        if (count2 > nums.size() / 3) res.push_back(cand2);
        return res;
    }
};
