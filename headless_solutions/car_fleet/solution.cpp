class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();
        vector<pair<int, int>> cars(n);
        for (int i = 0; i < n; ++i) {
            cars[i] = {position[i], speed[i]};
        }
        // Sort in descending order of position
        sort(cars.rbegin(), cars.rend());
        double max_time = 0.0;
        int count = 0;
        for (const auto& car : cars) {
            double time = (target - car.first) / (double)car.second;
            if (time > max_time) {
                max_time = time;
                count++;
            }
        }
        return count;
    }
};
