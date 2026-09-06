#include <vector>
#include <algorithm>
#include <queue>
#include <cfloat>

class Solution {
public:
    double mincostToHireWorkers(std::vector<int>& quality, std::vector<int>& wage, int k) {
        int n = quality.size();
        // Create pairs of (ratio, quality)
        std::vector<std::pair<double, int>> workers(n);
        for (int i = 0; i < n; ++i) {
            workers[i] = {(double)wage[i] / quality[i], quality[i]};
        }
        
        // Sort workers by wage-to-quality ratio ascending
        std::sort(workers.begin(), workers.end());
        
        std::priority_queue<int> maxHeap; // Max-heap to track highest qualities
        int qualitySum = 0;
        double minCost = DBL_MAX;
        
        for (const auto& worker : workers) {
            double ratio = worker.first;
            int q = worker.second;
            
            qualitySum += q;
            maxHeap.push(q);
            
            // If group size exceeds k, drop the worker with the highest quality
            if (maxHeap.size() > k) {
                qualitySum -= maxHeap.top();
                maxHeap.pop();
            }
            
            // Valid group of size k formed
            if (maxHeap.size() == k) {
                minCost = std::min(minCost, ratio * qualitySum);
            }
        }
        
        return minCost;
    }
};
