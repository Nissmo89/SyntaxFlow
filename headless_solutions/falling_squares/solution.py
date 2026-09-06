import collections
import functools
import itertools
import math
import heapq
import bisect
from typing import *
from collections import *
from functools import *
from heapq import *
from bisect import *

from typing import List
import bisect

class Solution:
    def fallingSquares(self, positions: List[List[int]]) -> List[int]:
        # Coordinate Compression
        coords = set()
        for left, sideLength in positions:
            coords.add(left)
            coords.add(left + sideLength) # Exclusive right boundary

        sorted_coords = sorted(list(coords))
        coord_to_idx = {coord: i for i, coord in enumerate(sorted_coords)}
        
        n_compressed = len(sorted_coords)
        # The segment tree will operate on indices 0 to n_compressed - 1.
        # Each leaf represents a compressed coordinate point.
        # The number of intervals is n_compressed - 1.
        # The segment tree will cover indices from 0 to n_compressed - 1.
        # The size of the tree array is typically 4 * number_of_leaves.
        tree_size = 4 * n_compressed
        tree = [[0, 0] for _ in range(tree_size)] # [max_height, lazy_tag]

        # Helper function to push lazy updates down the tree
        def push_down(node_idx: int, l: int, r: int):
            if tree[node_idx][1] != 0 and l != r: # If there's a lazy tag and it's not a leaf
                mid = l + (r - l) // 2
                left_child_idx = 2 * node_idx + 1
                right_child_idx = 2 * node_idx + 2

                # Apply lazy tag to children
                tree[left_child_idx][0] = tree[node_idx][1] # max_height
                tree[left_child_idx][1] = tree[node_idx][1] # lazy_tag

                tree[right_child_idx][0] = tree[node_idx][1] # max_height
                tree[right_child_idx][1] = tree[node_idx][1] # lazy_tag

                # Reset current node's lazy tag
                tree[node_idx][1] = 0

        # Range update function: sets the height in [update_l, update_r] to new_height
        # node_idx: current node index in the tree array
        # l, r: range represented by the current node (indices in sorted_coords)
        # update_l, update_r: the query range (indices in sorted_coords)
        # new_height: the height to set for the updated range
        def update_range(node_idx: int, l: int, r: int, update_l: int, update_r: int, new_height: int):
            # If current node's range is completely outside the update range
            if r < update_l or l > update_r:
                return

            # If current node's range is completely inside the update range
            if update_l <= l and r <= update_r:
                tree[node_idx][0] = new_height # max_height
                tree[node_idx][1] = new_height # lazy_tag
                return

            # Partially overlapping range, push down lazy tag and recurse
            push_down(node_idx, l, r)
            mid = l + (r - l) // 2
            update_range(2 * node_idx + 1, l, mid, update_l, update_r, new_height)
            update_range(2 * node_idx + 2, mid + 1, r, update_l, update_r, new_height)

            # Update current node's max_height based on children
            tree[node_idx][0] = max(tree[2 * node_idx + 1][0], tree[2 * node_idx + 2][0])

        # Range query function: finds max height in [query_l, query_r]
        # node_idx: current node index in the tree array
        # l, r: range represented by the current node (indices in sorted_coords)
        # query_l, query_r: the query range (indices in sorted_coords)
        def query_range(node_idx: int, l: int, r: int, query_l: int, query_r: int) -> int:
            # If current node's range is completely outside the query range
            if r < query_l or l > query_r:
                return 0 # Return 0 as it doesn't contribute to max height

            # If current node's range is completely inside the query range
            if query_l <= l and r <= query_r:
                return tree[node_idx][0]

            # Partially overlapping range, push down lazy tag and recurse
            push_down(node_idx, l, r)
            mid = l + (r - l) // 2
            left_max = query_range(2 * node_idx + 1, l, mid, query_l, query_r)
            right_max = query_range(2 * node_idx + 2, mid + 1, r, query_l, query_r)

            return max(left_max, right_max)

        ans = []
        overall_max_height = 0

        for left, sideLength in positions:
            right = left + sideLength # Exclusive right boundary

            # Get compressed indices for the interval [left, right)
            # The interval covers points from 'left' up to 'right - 1'.
            # The corresponding indices in sorted_coords are from coord_to_idx[left]
            # up to coord_to_idx[right] - 1.
            compressed_left_idx = coord_to_idx[left]
            compressed_right_idx = coord_to_idx[right] # Index of the coordinate 'right'

            # The segment tree operates on indices 0 to n_compressed - 1.
            # The range of indices for the square is [compressed_left_idx, compressed_right_idx - 1].
            # We query the max height in this range of indices.
            base_height = query_range(0, 0, n_compressed - 1, compressed_left_idx, compressed_right_idx - 1)

            new_height = base_height + sideLength

            # Update the interval [compressed_left_idx, compressed_right_idx - 1] with the new_height
            update_range(0, 0, n_compressed - 1, compressed_left_idx, compressed_right_idx - 1, new_height)

            # Update the overall maximum height
            overall_max_height = max(overall_max_height, new_height)
            ans.append(overall_max_height)

        return ans
