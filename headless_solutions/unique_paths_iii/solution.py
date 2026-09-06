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

class Solution:
    def uniquePathsIII(self, grid: List[List[int]]) -> int:
        m, n = len(grid), len(grid[0])
        empty = 0
        sx, sy = 0, 0
        for r in range(m):
            for c in range(n):
                if grid[r][c] == 1: sx, sy = r, c
                elif grid[r][c] == 0: empty += 1
        
        def dfs(x, y, count):
            if not (0 <= x < m and 0 <= y < n) or grid[x][y] == -1: return 0
            if grid[x][y] == 2: return 1 if count == empty + 1 else 0
            
            grid[x][y] = -1
            res = dfs(x+1, y, count+1) + dfs(x-1, y, count+1) + \
                  dfs(x, y+1, count+1) + dfs(x, y-1, count+1)
            grid[x][y] = 0
            return res
            
        return dfs(sx, sy, 0)
