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
from collections import Counter, defaultdict
from typing import List

class Solution:
    def numSquarefulPerms(self, nums: List[int]) -> int:
        count = Counter(nums)
        
        def is_square(n: int) -> bool:
            r = math.isqrt(n)
            return r * r == n

        # Build adjacency graph of valid squareful pairs
        graph = defaultdict(list)
        for u in count:
            for v in count:
                if is_square(u + v):
                    graph[u].append(v)

        # Backtracking DFS
        def dfs(x: int, left: int) -> int:
            count[x] -= 1
            if left == 0:
                ans = 1
            else:
                ans = sum(dfs(y, left - 1) for y in graph[x] if count[y] > 0)
            count[x] += 1  # Backtrack
            return ans

        return sum(dfs(x, len(nums) - 1) for x in count)
