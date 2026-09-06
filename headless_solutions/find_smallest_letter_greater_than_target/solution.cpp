class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        if (target >= letters.back() || target < letters.front()) {
            return letters.front();
        }
        
        int low = 0;
        int high = letters.size() - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (target >= letters[mid]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return letters[low];
    }
};
