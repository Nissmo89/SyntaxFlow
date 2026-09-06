class Solution {
public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> dict(wordList.begin(), wordList.end());
        unordered_map<string, int> dist;
        unordered_map<string, vector<string>> adj;
        
        queue<string> q;
        q.push(beginWord);
        dist[beginWord] = 0;
        
        bool found = false;
        while (!q.empty()) {
            string curr = q.front(); q.pop();
            if (curr == endWord) found = true;
            for (int i = 0; i < curr.size(); ++i) {
                string next = curr;
                for (char c = 'a'; c <= 'z'; ++c) {
                    next[i] = c;
                    if (dict.count(next)) {
                        if (dist.find(next) == dist.end()) {
                            dist[next] = dist[curr] + 1;
                            q.push(next);
                            adj[curr].push_back(next);
                        } else if (dist[next] == dist[curr] + 1) {
                            adj[curr].push_back(next);
                        }
                    }
                }
            }
        }
        
        vector<vector<string>> res;
        vector<string> path = {beginWord};
        if (found) dfs(beginWord, endWord, adj, dist, path, res);
        return res;
    }
    
    void dfs(string& curr, string& end, unordered_map<string, vector<string>>& adj, 
             unordered_map<string, int>& dist, vector<string>& path, vector<vector<string>>& res) {
        if (curr == end) { res.push_back(path); return; }
        for (const string& next : adj[curr]) {
            if (dist[next] == dist[curr] + 1) {
                path.push_back(next);
                dfs(const_cast<string&>(next), end, adj, dist, path, res);
                path.pop_back();
            }
        }
    }
};
