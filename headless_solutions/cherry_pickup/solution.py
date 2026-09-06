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
    def cherryPickup(self, grid: List[List[int]]) -> int:
        n = len(grid)
        INF = -10**9                     # unreachable sentinel

        # prev[r1][r2] corresponds to step t-1
        prev = [[INF] * n for _ in range(n)]
        prev[0][0] = grid[0][0] if grid[0][0] != -1 else INF

        # iterate over steps t = 1 .. 2n-2
        for t in range(1, 2 * n - 1):
            cur = [[INF] * n for _ in range(n)]
            r1_min = max(0, t - (n - 1))
            r1_max = min(n - 1, t)
            for r1 in range(r1_min, r1_max + 1):
                c1 = t - r1
                if grid[r1][c1] == -1:
                    continue
                r2_min = max(0, t - (n - 1))
                r2_max = min(n - 1, t)
                for r2 in range(r2_min, r2_max + 1):
                    c2 = t - r2
                    if grid[r2][c2] == -1:
                        continue

                    cherries = grid[r1][c1]
                    if r1 != r2 or c1 != c2:
                        cherries += grid[r2][c2]

                    best = INF
                    if r1 > 0 and r2 > 0:
                        best = max(best, prev[r1-1][r2-1])
                    if r1 > 0:
                        best = max(best, prev[r1-1][r2])
                    if r2 > 0:
                        best = max(best, prev[r1][r2-1])
                    best = max(best, prev[r1][r2])

                    if best != INF:
                        cur[r1][r2] = best + cherries
            prev = cur

        ans = prev[n-1][n-1]
        return ans if ans > 0 else 0
