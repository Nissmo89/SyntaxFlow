class Solution {
public:
    int numRescueBoats(vector<int>& people, int limit) {
        // Sort weights to enable two-pointer greedy pairing
        sort(people.begin(), people.end());
        
        int left = 0;
        int right = people.size() - 1;
        int boats = 0;
        
        while (left <= right) {
            // Try to pair lightest with heaviest
            if (people[left] + people[right] <= limit) {
                left++;   // Lightest person boards
            }
            right--;      // Heaviest person always boards (alone or paired)
            boats++;
        }
        return boats;
    }
};
