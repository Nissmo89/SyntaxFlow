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
    def shortestBridge(self, grid: List[List[int]]) -> int:
        n = len(grid)
        q = deque()
        def dfs(r, c):
            if 0 <= r < n and 0 <= c < n and grid[r][c] == 1:
                grid[r][c] = 2
                q.append((r, c))
                for dr, dc in [(0,1), (0,-1), (1,0), (-1,0)]:
                    dfs(r + dr, c + dc)
        
        found = False
        for i in range(n):
            for j in range(n):
                if grid[i][j] == 1:
                    dfs(i, j)
                    found = True; break
            if found: break
            
        dist = 0
        while q:
            for _ in range(len(q)):
                r, c = q.popleft()
                for dr, dc in [(0,1), (0,-1), (1,0), (-1,0)]:
                    nr, nc = r + dr, c + dc
                    if 0 <= nr < n and 0 <= nc < n:
                        if grid[nr][nc] == 1: return dist
                        if grid[nr][nc] == 0:
                            grid[nr][nc] = 2
                            q.append((nr, nc))
            dist += 1
        return -1
