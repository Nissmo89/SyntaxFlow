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

import math
from typing import List
from collections import defaultdict

class Solution:
    def maxPoints(self, points: List[List[int]]) -> int:
        n = len(points)
        if n <= 2:
            return n

        max_overall = 1

        for i in range(n):
            slope_counts = defaultdict(int)
            local_max = 0
            x1, y1 = points[i]

            for j in range(i + 1, n):
                x2, y2 = points[j]
                dy = y2 - y1
                dx = x2 - x1

                g = math.gcd(abs(dy), abs(dx))
                dy //= g
                dx //= g

                # Normalize sign representation
                if dx < 0 or (dx == 0 and dy < 0):
                    dx = -dx
                    dy = -dy

                slope_counts[(dy, dx)] += 1
                if slope_counts[(dy, dx)] > local_max:
                    local_max = slope_counts[(dy, dx)]

            max_overall = max(max_overall, local_max + 1)

        return max_overall
