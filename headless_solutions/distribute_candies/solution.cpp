class Solution {
public:
    int distributeCandies(vector<int>& candyType) {
        unordered_set<int> uniqueCandies(candyType.begin(), candyType.end());
        return min(uniqueCandies.size(), candyType.size() / 2);
    }
};
