class Solution {
public:
    bool divisorGame(int n) {
        // If n is even, Alice wins
        return n % 2 == 0;
    }
};
