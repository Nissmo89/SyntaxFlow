#include <vector>
#include <algorithm>
#include <set>

class Solution {
public:
    // Helper function to compute the cross product of vectors OA and OB.
    // Returns positive for counter-clockwise turn, negative for clockwise, zero for collinear.
    long long cross_product(const std::vector<int>& O, const std::vector<int>& A, const std::vector<int>& B) {
        // (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x)
        long long val = (long long)(A[0] - O[0]) * (B[1] - O[1]) - (long long)(A[1] - O[1]) * (B[0] - O[0]);
        return val;
    }

    std::vector<std::vector<int>> outerTrees(std::vector<std::vector<int>>& trees) {
        int n = trees.size();
        // If there are 3 or fewer trees, all are on the perimeter.
        if (n <= 3) {
            return trees;
        }

        // Sort points lexicographically: first by x, then by y.
        std::sort(trees.begin(), trees.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
            if (a[0] != b[0]) {
                return a[0] < b[0];
            }
            return a[1] < b[1];
        });

        std::vector<std::vector<int>> lower_hull;
        std::vector<std::vector<int>> upper_hull;

        // Build lower hull
        for (const auto& tree : trees) {
            // While the last two points in lower_hull and the current tree make a clockwise turn (or are collinear and we want to keep only extreme points for strict convex hull, but here we need all collinear points on boundary)
            // For including collinear points on the boundary, we use <= 0.
            // If cross_product is < 0, it's a right turn. If it's 0, they are collinear.
            // We pop if it's a right turn. If it's collinear, we might pop the middle point if we want a strict convex hull.
            // To include all collinear points on the boundary, we pop only on strict right turns (< 0).
            while (lower_hull.size() >= 2 && cross_product(lower_hull[lower_hull.size() - 2], lower_hull.back(), tree) < 0) {
                lower_hull.pop_back();
            }
            lower_hull.push_back(tree);
        }

        // Build upper hull
        // Iterate in reverse order
        for (int i = n - 1; i >= 0; --i) {
            const auto& tree = trees[i];
            // Similar logic for upper hull, maintaining counter-clockwise turns when viewed from above.
            // Popping on strict right turns (< 0).
            while (upper_hull.size() >= 2 && cross_product(upper_hull[upper_hull.size() - 2], upper_hull.back(), tree) < 0) {
                upper_hull.pop_back();
            }
            upper_hull.push_back(tree);
        }

        // Combine hulls and remove duplicates.
        // Using a set to automatically handle duplicates.
        std::set<std::vector<int>> hull_set;

        // Add all points from lower hull
        for (const auto& p : lower_hull) {
            hull_set.insert(p);
        }
        // Add all points from upper hull
        for (const auto& p : upper_hull) {
            hull_set.insert(p);
        }

        // Convert set back to vector of vectors
        std::vector<std::vector<int>> result;
        for (const auto& p : hull_set) {
            result.push_back(p);
        }

        return result;
    }
};
