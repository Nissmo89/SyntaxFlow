class Solution {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumAlice = 0, sumBob = 0;
        for (int size : aliceSizes) sumAlice += size;
        for (int size : bobSizes) sumBob += size;
        
        int dif = (sumAlice - sumBob) / 2;
        unordered_set<int> aliceSet(aliceSizes.begin(), aliceSizes.end());
        
        for (int bobSize : bobSizes) {
            if (aliceSet.count(bobSize + dif)) {
                return {bobSize + dif, bobSize};
            }
        }
        
        // This should not happen given the problem constraints
        return {};
    }
};
