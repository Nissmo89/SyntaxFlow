class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int n = difficulty.size();
        vector<pair<int, int>> jobs(n);
        for (int i = 0; i < n; ++i) jobs[i] = {difficulty[i], profit[i]};
        sort(jobs.begin(), jobs.end());
        sort(worker.begin(), worker.end());
        
        int totalProfit = 0, i = 0, bestProfit = 0;
        for (int ability : worker) {
            while (i < n && ability >= jobs[i].first) {
                bestProfit = max(bestProfit, jobs[i].second);
                i++;
            }
            totalProfit += bestProfit;
        }
        return totalProfit;
    }
};
