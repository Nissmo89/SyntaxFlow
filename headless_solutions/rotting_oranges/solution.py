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

from collections import deque

class Solution:
    def orangesRotting(self, grid: List[List[int]]) -> int:
        m, n, fresh = len(grid), len(grid[0]), 0
        q = deque()
        for r in range(m):
            for c in range(n):
                if grid[r][c] == 2: q.append((r, c))
                elif grid[r][c] == 1: fresh += 1
        
        if fresh == 0: return 0
        minutes = 0
        while q and fresh > 0:
            minutes += 1
            for _ in range(len(q)):
                r, c = q.popleft()
                for dr, dc in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
                    nr, nc = r + dr, c + dc
                    if 0 <= nr < m and 0 <= nc < n and grid[nr][nc] == 1:
                        grid[nr][nc] = 2
                        fresh -= 1
                        q.append((nr, nc))
        return minutes if fresh == 0 else -1
