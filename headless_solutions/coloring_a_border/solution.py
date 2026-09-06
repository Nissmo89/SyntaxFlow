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
    def colorBorder(self, grid: List[List[int]], row: int, col: int, color: int) -> List[List[int]]:
        if not grid:
            return grid
        m, n = len(grid), len(grid[0])
        original = grid[row][col]
        # Early exit: colouring with the same colour changes nothing.
        if original == color:
            return grid

        visited = [[False] * n for _ in range(m)]
        component = []
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

        # -------- DFS to gather the component --------
        def dfs(r: int, c: int) -> None:
            if not (0 <= r < m and 0 <= c < n):
                return
            if visited[r][c] or grid[r][c] != original:
                return
            visited[r][c] = True
            component.append((r, c))
            for dr, dc in directions:
                dfs(r + dr, c + dc)

        dfs(row, col)

        # -------- Determine border cells and recolour --------
        for r, c in component:
            is_border = False
            for dr, dc in directions:
                nr, nc = r + dr, c + dc
                if not (0 <= nr < m and 0 <= nc < n) or grid[nr][nc] != original:
                    is_border = True
                    break
            if is_border:
                grid[r][c] = color
        return grid
