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
    def shortestPathAllKeys(self, grid: list[str]) -> int:
        m, n = len(grid), len(grid[0])
        keys_count = 0
        start_pos = (0, 0)
        for r in range(m):
            for c in range(n):
                if grid[r][c] == '@': start_pos = (r, c)
                elif 'a' <= grid[r][c] <= 'f': keys_count += 1
        
        target = (1 << keys_count) - 1
        queue = deque([(start_pos[0], start_pos[1], 0, 0)]) # r, c, mask, dist
        visited = {(start_pos[0], start_pos[1], 0)}
        
        while queue:
            r, c, mask, dist = queue.popleft()
            if mask == target: return dist
            for dr, dc in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
                nr, nc = r + dr, c + dc
                if 0 <= nr < m and 0 <= nc < n and grid[nr][nc] != '#':
                    char = grid[nr][nc]
                    nmask = mask
                    if 'a' <= char <= 'f': nmask |= (1 << (ord(char) - ord('a')))
                    if 'A' <= char <= 'F' and not (mask & (1 << (ord(char) - ord('A')))): continue
                    if (nr, nc, nmask) not in visited:
                        visited.add((nr, nc, nmask))
                        queue.append((nr, nc, nmask, dist + 1))
        return -1
