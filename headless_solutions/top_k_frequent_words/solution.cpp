class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        unordered_map<string, int> count;
        for (const string& w : words) count[w]++;
        
        auto comp = [](pair<int, string>& a, pair<int, string>& b) {
            if (a.first != b.first) return a.first > b.first;
            return a.second < b.second;
        };
        
        priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(comp)> pq(comp);
        for (auto& entry : count) {
            pq.push({entry.second, entry.first});
            if (pq.size() > k) pq.pop();
        }
        
        vector<string> res(k);
        for (int i = k - 1; i >= 0; --i) {
            res[i] = pq.top().second;
            pq.pop();
        }
        return res;
    }
};
