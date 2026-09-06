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
    def hitBricks(self, grid: List[List[int]], hits: List[List[int]]) -> List[int]:
        R, C = len(grid), len(grid[0])
        parent = list(range(R * C + 1))
        size = [1] * (R * C + 1)
        
        def find(i):
            if parent[i] == i: return i
            parent[i] = find(parent[i])
            return parent[i]
            
        def union(i, j):
            root_i, root_j = find(i), find(j)
            if root_i != root_j:
                parent[root_i] = root_j
                size[root_j] += size[root_i]
        
        status = [row[:] for row in grid]
        for r, c in hits: status[r][c] = 0
        
        for r in range(R):
            for c in range(C):
                if status[r][c]:
                    if r == 0: union(r * C + c + 1, 0)
                    if r > 0 and status[r-1][c]: union(r * C + c + 1, (r-1) * C + c + 1)
                    if c > 0 and status[r][c-1]: union(r * C + c + 1, r * C + c)
        
        res = [0] * len(hits)
        for i in range(len(hits) - 1, -1, -1):
            r, c = hits[i]
            if grid[r][c] == 0: continue
            prev = size[find(0)]
            status[r][c] = 1
            for dr, dc in [(0,1),(0,-1),(1,0),(-1,0)]:
                nr, nc = r + dr, c + dc
                if 0 <= nr < R and 0 <= nc < C and status[nr][nc]:
                    union(r * C + c + 1, nr * C + nc + 1)
            if r == 0: union(r * C + c + 1, 0)
            res[i] = max(0, size[find(0)] - prev - 1)
        return res
