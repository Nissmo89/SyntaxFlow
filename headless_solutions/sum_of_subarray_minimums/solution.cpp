class Solution {
public:
    int sumSubarrayMins(vector<int>& arr) {
        int n = arr.size();
        long long MOD = 1e9 + 7;
        vector<int> left(n), right(n);
        stack<int> s;

        for (int i = 0; i < n; ++i) {
            while (!s.empty() && arr[s.top()] > arr[i]) s.pop();
            left[i] = s.empty() ? i + 1 : i - s.top();
            s.push(i);
        }
        while (!s.empty()) s.pop();
        for (int i = n - 1; i >= 0; --i) {
            while (!s.empty() && arr[s.top()] >= arr[i]) s.pop();
            right[i] = s.empty() ? n - i : s.top() - i;
            s.push(i);
        }

        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            ans = (ans + (long long)arr[i] * left[i] * right[i]) % MOD;
        }
        return ans;
    }
};
