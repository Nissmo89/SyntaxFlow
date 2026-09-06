class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int total_surplus = 0, current_tank = 0, start_index = 0;
        for (int i = 0; i < gas.size(); ++i) {
            int net = gas[i] - cost[i];
            total_surplus += net;
            current_tank += net;
            if (current_tank < 0) {
                start_index = i + 1;
                current_tank = 0;
            }
        }
        return total_surplus < 0 ? -1 : start_index;
    }
};
