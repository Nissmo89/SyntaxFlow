#include <vector>
#include <queue>
#include <algorithm> // For std::sort

class Solution {
public:
    int findMaximizedCapital(int k, int w, std::vector<int>& profits, std::vector<int>& capital) {
        int n = profits.size();
        // Create a vector of pairs to store (capital, profit) for each project.
        // This allows sorting by capital while keeping profit associated.
        std::vector<std::pair<int, int>> projects(n);
        for (int i = 0; i < n; ++i) {
            projects[i] = {capital[i], profits[i]};
        }

        // Sort projects by their capital requirements in ascending order.
        std::sort(projects.begin(), projects.end());

        // Max-priority queue to store profits of affordable projects.
        // std::priority_queue is a max-heap by default for primitive types.
        std::priority_queue<int> max_profit_heap;

        long long current_capital = w; // Use long long to prevent potential overflow for w
                                       // although problem states final answer fits 32-bit int.
                                       // Intermediate sums can exceed 2*10^9.
        int project_idx = 0; // Pointer for the sorted projects array

        // Perform at most k projects
        for (int i = 0; i < k; ++i) {
            // Add all affordable projects to the max-profit heap
            while (project_idx < n && projects[project_idx].first <= current_capital) {
                max_profit_heap.push(projects[project_idx].second);
                project_idx++;
            }

            // If no projects are affordable, we can't do any more.
            if (max_profit_heap.empty()) {
                break;
            }

            // Pick the project with the maximum profit from the heap
            current_capital += max_profit_heap.top();
            max_profit_heap.pop();
        }

        return static_cast<int>(current_capital);
    }
};
