class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        unordered_map<string, int> map;
        for (int i = 0; i < list1.size(); i++) {
            map[list1[i]] = i;
        }
        int minSum = INT_MAX;
        vector<string> res;
        for (int i = 0; i < list2.size(); i++) {
            if (map.find(list2[i]) != map.end()) {
                int sum = map[list2[i]] + i;
                if (sum < minSum) {
                    minSum = sum;
                    res.clear();
                    res.push_back(list2[i]);
                } else if (sum == minSum) {
                    res.push_back(list2[i]);
                }
            }
        }
        return res;
    }
};
