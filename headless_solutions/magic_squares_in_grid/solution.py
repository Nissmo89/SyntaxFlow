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
    def numMagicSquaresInside(self, grid: List[List[int]]) -> int:
        def is_magic(r, c):
            s = [grid[r+i][c+j] for i in range(3) for j in range(3)]
            if sorted(s) != list(range(1, 10)): return False
            sums = [sum(grid[r+i][c:c+3]) for i in range(3)] + \
                   [sum(grid[r+i][c+j] for i in range(3)) for j in range(3)] + \
                   [grid[r][c] + grid[r+1][c+1] + grid[r+2][c+2], 
                    grid[r][c+2] + grid[r+1][c+1] + grid[r+2][c]]
            return all(x == 15 for x in sums)

        rows, cols = len(grid), len(grid[0])
        return sum(is_magic(i, j) for i in range(rows - 2) for j in range(cols - 2))
