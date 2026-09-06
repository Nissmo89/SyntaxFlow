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
    def matrixScore(self, grid: List[List[int]]) -> int:
        M, N = len(grid), len(grid[0])
        # Initial score from the first column (all 1s)
        res = (1 << (N - 1)) * M
        
        for j in range(1, N):
            # Count how many rows have a 1 in this column
            # If grid[i][0] is 0, the row was flipped, so 1 becomes 0 and 0 becomes 1
            count = sum(1 for i in range(M) if grid[i][j] == grid[i][0])
            res += max(count, M - count) * (1 << (N - 1 - j))
            
        return res
