#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> numMovesStonesII(vector<int>& stones) {
        sort(stones.begin(), stones.end());
        int n = stones.size();
        int low = n;
        int high = max(stones[n - 1] - stones[1] - n + 2, stones[n - 2] - stones[0] - n + 2);
        
        int i = 0;
        for (int j = 0; j < n; ++j) {
            while (stones[j] - stones[i] >= n) i++;
            int count = j - i + 1;
            if (count == n - 1 && stones[j] - stones[i] == n - 2)
                low = min(low, 2);
            else
                low = min(low, n - count);
        }
        return {low, high};
    }
};
