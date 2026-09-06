class Solution {
public:
    int numPairsDivisibleBy60(vector<int>& time) {
        vector<int> count(60, 0);
        int total_pairs = 0;
        for (int t : time) {
            int rem = t % 60;
            int target = (60 - rem) % 60;
            total_pairs += count[target];
            count[rem]++;
        }
        return total_pairs;
    }
};
