class Solution {
public:
    int numFriendRequests(vector<int>& ages) {
        vector<int> count(121, 0);
        for (int age : ages) count[age]++;
        
        vector<int> prefix(121, 0);
        for (int i = 1; i <= 120; ++i) prefix[i] = prefix[i - 1] + count[i];
        
        int total = 0;
        for (int x = 1; x <= 120; ++x) {
            if (count[x] == 0) continue;
            int min_y = x / 2 + 7 + 1;
            if (x < min_y) continue;
            
            int num_targets = prefix[x] - prefix[min_y - 1];
            // Subtract self if self is in range
            if (x >= min_y && x <= x) num_targets--;
            
            total += num_targets * count[x];
        }
        return total;
    }
};
