class Solution {
public:
    int findMinMoves(vector<int>& machines) {
        int n = machines.size();
        int total = accumulate(machines.begin(), machines.end(), 0);
        if (total % n != 0) return -1;
        
        int target = total / n;
        int balance = 0, max_moves = 0;
        
        for (int load : machines) {
            balance += (load - target);
            // max_moves is max of:
            // 1. Current absolute balance (flow across boundary)
            // 2. The load of the current machine (if it needs to export)
            max_moves = max({max_moves, abs(balance), load - target});
        }
        return max_moves;
    }
};
