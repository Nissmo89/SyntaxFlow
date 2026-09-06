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
    def leastBricks(self, wall: List[List[int]]) -> int:
        edge_counts = {}
        max_count = 0
        for row in wall:
            current_sum = 0
            for brick in row[:-1]:  # Exclude last brick
                current_sum += brick
                edge_counts[current_sum] = edge_counts.get(current_sum, 0) + 1
                if edge_counts[current_sum] > max_count:
                    max_count = edge_counts[current_sum]
        return len(wall) - max_count
