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

class Solution:
    def outerTrees(self, trees: List[List[int]]) -> List[List[int]]:
        n = len(trees)
        # If there are 3 or fewer trees, all are on the perimeter.
        if n <= 3:
            return trees

        # Helper function to compute the cross product of vectors OA and OB.
        # Returns positive for counter-clockwise turn, negative for clockwise, zero for collinear.
        def cross_product(O, A, B):
            # (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x)
            return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0])

        # Sort points lexicographically: first by x, then by y.
        trees.sort()

        lower_hull = []
        upper_hull = []

        # Build lower hull
        for tree in trees:
            # While the last two points in lower_hull and the current tree make a clockwise turn (< 0)
            # We pop only on strict right turns to include collinear points on the boundary.
            while len(lower_hull) >= 2 and cross_product(lower_hull[-2], lower_hull[-1], tree) < 0:
                lower_hull.pop()
            lower_hull.append(tree)

        # Build upper hull
        # Iterate in reverse order
        for tree in reversed(trees):
            # Similar logic for upper hull, maintaining counter-clockwise turns when viewed from above.
            # Popping on strict right turns (< 0).
            while len(upper_hull) >= 2 and cross_product(upper_hull[-2], upper_hull[-1], tree) < 0:
                upper_hull.pop()
            upper_hull.append(tree)

        # Combine hulls and remove duplicates.
        # Using a set of tuples to automatically handle duplicates.
        hull_set = set()

        # Add all points from lower hull
        for p in lower_hull:
            hull_set.add(tuple(p))
        # Add all points from upper hull
        for p in upper_hull:
            hull_set.add(tuple(p))

        # Convert set of tuples back to list of lists
        result = [list(p) for p in hull_set]

        return result
