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

import heapq

class Solution:
    def trapRainWater(self, heightMap: List[List[int]]) -> int:
        if not heightMap or not heightMap[0]: return 0
        m, n = len(heightMap), len(heightMap[0])
        pq, visited = [], [[False for _ in range(n)] for _ in range(m)]
        
        for r in range(m):
            for c in range(n):
                if r == 0 or r == m - 1 or c == 0 or c == n - 1:
                    heapq.heappush(pq, (heightMap[r][c], r, c))
                    visited[r][c] = True
                    
        res, dirs = 0, [(0, 1), (0, -1), (1, 0), (-1, 0)]
        while pq:
            h, r, c = heapq.heappop(pq)
            for dr, dc in dirs:
                nr, nc = r + dr, c + dc
                if 0 <= nr < m and 0 <= nc < n and not visited[nr][nc]:
                    res += max(0, h - heightMap[nr][nc])
                    heapq.heappush(pq, (max(h, heightMap[nr][nc]), nr, nc))
                    visited[nr][nc] = True
        return res
