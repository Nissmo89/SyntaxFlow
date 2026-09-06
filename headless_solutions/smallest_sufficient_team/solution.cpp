class Solution {
public:
    vector<int> smallestSufficientTeam(vector<string>& req_skills, vector<vector<string>>& people) {
        int n = req_skills.size(), m = people.size();
        unordered_map<string, int> skill_to_id;
        for (int i = 0; i < n; ++i) skill_to_id[req_skills[i]] = i;

        vector<vector<int>> dp(1 << n);
        dp[0] = {};

        for (int i = 0; i < m; ++i) {
            int person_mask = 0;
            for (const string& s : people[i]) person_mask |= (1 << skill_to_id[s]);
            
            for (int mask = 0; mask < (1 << n); ++mask) {
                if (dp[mask].empty() && mask != 0) continue;
                int next_mask = mask | person_mask;
                if (next_mask == mask) continue;
                if (dp[next_mask].empty() || dp[next_mask].size() > dp[mask].size() + 1) {
                    dp[next_mask] = dp[mask];
                    dp[next_mask].push_back(i);
                }
            }
        }
        return dp[(1 << n) - 1];
    }
};
