class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        vector<int> answer(n, 0);
        vector<int> stack; // Stores indices
        for (int i = 0; i < n; ++i) {
            while (!stack.empty() && temperatures[i] > temperatures[stack.back()]) {
                int prev_idx = stack.back();
                stack.pop_back();
                answer[prev_idx] = i - prev_idx;
            }
            stack.push_back(i);
        }
        return answer;
    }
};
