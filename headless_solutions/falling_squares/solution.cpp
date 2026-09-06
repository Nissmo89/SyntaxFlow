#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

// Structure for Segment Tree Node
struct Node {
    int max_height; // Maximum height in the range represented by this node
    int lazy_tag;   // Pending update value (the new height to set)
};

class Solution {
private:
    vector<Node> tree;
    vector<int> compressed_coords;
    map<int, int> coord_to_idx;
    int num_intervals; // Number of elementary intervals (compressed_coords.size() - 1)

    // Helper function to push lazy updates down the tree
    void push_down(int node_idx, int l, int r) {
        if (tree[node_idx].lazy_tag != 0 && l != r) { // If there's a lazy tag and it's not a leaf
            int mid = l + (r - l) / 2;
            int left_child_idx = 2 * node_idx + 1;
            int right_child_idx = 2 * node_idx + 2;

            // Apply lazy tag to children
            tree[left_child_idx].max_height = tree[node_idx].lazy_tag;
            tree[left_child_idx].lazy_tag = tree[node_idx].lazy_tag;

            tree[right_child_idx].max_height = tree[node_idx].lazy_tag;
            tree[right_child_idx].lazy_tag = tree[node_idx].lazy_tag;

            // Reset current node's lazy tag
            tree[node_idx].lazy_tag = 0;
        }
    }

    // Range update function: sets the height in [update_l, update_r] to new_height
    // node_idx: current node index in the tree vector
    // l, r: range represented by the current node (indices in compressed_coords)
    // update_l, update_r: the query range (indices in compressed_coords)
    // new_height: the height to set for the updated range
    void update_range(int node_idx, int l, int r, int update_l, int update_r, int new_height) {
        // If current node's range is completely outside the update range
        if (r < update_l || l > update_r) {
            return;
        }

        // If current node's range is completely inside the update range
        if (update_l <= l && r <= update_r) {
            tree[node_idx].max_height = new_height;
            tree[node_idx].lazy_tag = new_height; // Store the new height as lazy tag
            return;
        }

        // Partially overlapping range, push down lazy tag and recurse
        push_down(node_idx, l, r);
        int mid = l + (r - l) / 2;
        update_range(2 * node_idx + 1, l, mid, update_l, update_r, new_height);
        update_range(2 * node_idx + 2, mid + 1, r, update_l, update_r, new_height);

        // Update current node's max_height based on children
        tree[node_idx].max_height = max(tree[2 * node_idx + 1].max_height, tree[2 * node_idx + 2].max_height);
    }

    // Range query function: finds max height in [query_l, query_r]
    // node_idx: current node index in the tree vector
    // l, r: range represented by the current node (indices in compressed_coords)
    // query_l, query_r: the query range (indices in compressed_coords)
    int query_range(int node_idx, int l, int r, int query_l, int query_r) {
        // If current node's range is completely outside the query range
        if (r < query_l || l > query_r) {
            return 0; // Return 0 as it doesn't contribute to max height
        }

        // If current node's range is completely inside the query range
        if (query_l <= l && r <= query_r) {
            return tree[node_idx].max_height;
        }

        // Partially overlapping range, push down lazy tag and recurse
        push_down(node_idx, l, r);
        int mid = l + (r - l) / 2;
        int left_max = query_range(2 * node_idx + 1, l, mid, query_l, query_r);
        int right_max = query_range(2 * node_idx + 2, mid + 1, r, query_l, query_r);

        return max(left_max, right_max);
    }

public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        set<int> coords_set;
        for (const auto& pos : positions) {
            coords_set.insert(pos[0]); // left
            coords_set.insert(pos[0] + pos[1] - 1); // right edge (inclusive)
        }

        compressed_coords.assign(coords_set.begin(), coords_set.end());
        for (int i = 0; i < compressed_coords.size(); ++i) {
            coord_to_idx[compressed_coords[i]] = i;
        }

        // The segment tree operates on intervals. If we have K unique coordinates,
        // they define K-1 intervals. The indices for these intervals will be 0 to K-2.
        // So, the segment tree will cover indices from 0 to compressed_coords.size() - 2.
        num_intervals = compressed_coords.size(); // Number of points, not intervals
        // The segment tree will cover indices from 0 to num_intervals - 1.
        // Each leaf node represents a single compressed coordinate point.
        // We need to map intervals [coord_i, coord_{i+1}) to segment tree indices.
        // The number of effective intervals is compressed_coords.size() - 1.
        // Let's adjust the segment tree to cover indices from 0 to compressed_coords.size() - 1.
        // The range for update/query will be [coord_to_idx[left], coord_to_idx[right] - 1]
        // which corresponds to indices [compressed_left_idx, compressed_right_idx - 1].
        // The segment tree will operate on indices 0 to compressed_coords.size() - 1.
        // The number of leaves will be compressed_coords.size().
        // The effective range for intervals is [0, compressed_coords.size() - 2].
        // Let's use indices 0 to compressed_coords.size() - 1 for the segment tree leaves.
        // The range of indices for the segment tree will be [0, compressed_coords.size() - 1].
        // The number of leaves is compressed_coords.size().
        // The tree size needs to be roughly 4 * number_of_leaves.
        tree.resize(4 * compressed_coords.size(), {0, 0});

        vector<int> ans;
        int overall_max_height = 0;

        for (const auto& pos : positions) {
            int left = pos[0];
            int sideLength = pos[1];
            int right = left + sideLength; // Exclusive right boundary

            // Get compressed indices for the interval [left, right)
            // The interval is defined by coordinates from left up to (but not including) right.
            // We need to find the range of indices in `compressed_coords` that fall within [left, right).
            // `lower_bound` finds the first element >= left.
            // `upper_bound` finds the first element >= right.
            // The indices in `compressed_coords` that correspond to the interval [left, right)
            // are from `coord_to_idx[left]` up to `coord_to_idx[right-1]` if right-1 exists.
            // A simpler way: the interval [left, right) corresponds to compressed indices
            // from `coord_to_idx[left]` up to `coord_to_idx[right] - 1`.
            // The segment tree operates on indices of `compressed_coords`.
            // The interval [left, right) covers points from `left` up to `right-1`.
            // The compressed indices for these points are `coord_to_idx[left]` to `coord_to_idx[right-1]`.
            // The segment tree range for update/query should be [compressed_left_idx, compressed_right_idx - 1].
            // `compressed_left_idx` is `coord_to_idx[left]`.
            // `compressed_right_idx` is `coord_to_idx[right]`.
            // The range of indices in `compressed_coords` that are covered by the square is
            // from `coord_to_idx[left]` up to `coord_to_idx[right-1]`.
            // The segment tree indices for this range are `coord_to_idx[left]` to `coord_to_idx[right] - 1`.
            // Let's use `coord_to_idx[left]` as the start index and `coord_to_idx[right] - 1` as the end index for the segment tree.
            // The segment tree indices are 0 to compressed_coords.size() - 1.
            // The range of indices for the square is [coord_to_idx[left], coord_to_idx[right] - 1].
            
            int compressed_left_idx = coord_to_idx[left];
            // The right boundary is `left + sideLength`. The interval is [left, left + sideLength).
            // We need to find the index of the coordinate `right`.
            // The indices in `compressed_coords` that are part of the square's span are
            // from `coord_to_idx[left]` up to `coord_to_idx[right] - 1`.
            // The segment tree operates on indices of `compressed_coords`.
            // The range of indices for the square is [coord_to_idx[left], coord_to_idx[right] - 1].
            int compressed_right_idx = coord_to_idx[right]; // This is the index of the coordinate 'right'

            // Query for the maximum height in the interval [compressed_left_idx, compressed_right_idx - 1]
            // The segment tree operates on indices 0 to compressed_coords.size() - 1.
            // The range of indices for the square is [compressed_left_idx, compressed_right_idx - 1].
            int base_height = query_range(0, 0, compressed_coords.size() - 1, compressed_left_idx, compressed_right_idx - 1);

            int new_height = base_height + sideLength;

            // Update the interval [compressed_left_idx, compressed_right_idx - 1] with the new_height
            update_range(0, 0, compressed_coords.size() - 1, compressed_left_idx, compressed_right_idx - 1, new_height);

            // Update the overall maximum height
            overall_max_height = max(overall_max_height, new_height);
            ans.push_back(overall_max_height);
        }

        return ans;
    }
};
