class Solution {
public:
    int maxSatisfied(vector<int>& customers, vector<int>& grumpy, int minutes) {
        int total_satisfied = 0;
        int current_window_gain = 0;
        int max_window_gain = 0;

        for (int i = 0; i < customers.size(); ++i) {
            if (grumpy[i] == 0) total_satisfied += customers[i];
            else current_window_gain += customers[i];

            if (i >= minutes && grumpy[i - minutes] == 1) {
                current_window_gain -= customers[i - minutes];
            }
            max_window_gain = max(max_window_gain, current_window_gain);
        }
        return total_satisfied + max_window_gain;
    }
};
