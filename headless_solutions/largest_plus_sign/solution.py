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
    def orderOfLargestPlusSign(self, n: int, mines: List[List[int]]) -> int:
        grid = [[n] * n for _ in range(n)]
        for r, c in mines:
            grid[r][c] = 0
            
        for i in range(n):
            l, r, u, d = 0, 0, 0, 0
            for j in range(n):
                l = l + 1 if grid[i][j] else 0
                grid[i][j] = min(grid[i][j], l)
                
                r = r + 1 if grid[i][n-1-j] else 0
                grid[i][n-1-j] = min(grid[i][n-1-j], r)
                
                u = u + 1 if grid[j][i] else 0
                grid[j][i] = min(grid[j][i], u)
                
                d = d + 1 if grid[n-1-j][i] else 0
                grid[n-1-j][i] = min(grid[n-1-j][i], d)
                
        return max(max(row) for row in grid)
