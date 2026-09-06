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
    def pacificAtlantic(self, heights: List[List[int]]) -> List[List[int]]:
        m, n = len(heights), len(heights[0])
        pac, atl = set(), set()
        def dfs(r, c, visited, prev_h):
            if (r, c) in visited or not (0 <= r < m and 0 <= c < n) or heights[r][c] < prev_h:
                return
            visited.add((r, c))
            for dr, dc in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
                dfs(r + dr, c + dc, visited, heights[r][c])
        for i in range(m):
            dfs(i, 0, pac, heights[i][0])
            dfs(i, n - 1, atl, heights[i][n - 1])
        for j in range(n):
            dfs(0, j, pac, heights[0][j])
            dfs(m - 1, j, atl, heights[m - 1][j])
        return list(map(list, pac & atl))
