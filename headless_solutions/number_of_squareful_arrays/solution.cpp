#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

class Solution {
private:
    // Helper function to check if a sum is a perfect square
    bool isSquare(long long val) {
        long long root = static_cast<long long>(sqrt(val));
        return root * root == val;
    }

    // DFS backtracking over unique values
    int dfs(int x, int left, unordered_map<int, int>& count, unordered_map<int, vector<int>>& graph) {
        count[x]--;
        int validPaths = 0;
        
        if (left == 0) {
            validPaths = 1;
        } else {
            for (int neighbor : graph[x]) {
                if (count[neighbor] > 0) {
                    validPaths += dfs(neighbor, left - 1, count, graph);
                }
            }
        }
        
        count[x]++; // Backtrack
        return validPaths;
    }

public:
    int numSquarefulPerms(vector<int>& nums) {
        unordered_map<int, int> count;
        for (int x : nums) {
            count[x]++;
        }

        // Build adjacency graph of valid squareful pairs
        unordered_map<int, vector<int>> graph;
        for (const auto& p1 : count) {
            for (const auto& p2 : count) {
                int u = p1.first;
                int v = p2.first;
                if (isSquare((long long)u + v)) {
                    graph[u].push_back(v);
                }
            }
        }

        // Start DFS from each unique starting element
        int totalPermutations = 0;
        for (const auto& p : count) {
            totalPermutations += dfs(p.first, nums.size() - 1, count, graph);
        }

        return totalPermutations;
    }
};
