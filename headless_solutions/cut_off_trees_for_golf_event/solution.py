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
       def cutOffTree(self, forest: List[List[int]]) -> int:
           m, n = len(forest), len(forest[0])
           trees = sorted((v, r, c) for r, row in enumerate(forest) for c, v in enumerate(row) if v > 1)

           sr, sc, ans = 0, 0, 0
           dr, dc = [-1, 1, 0, 0], [0, 0, -1, 1]

           for _, tr, tc in trees:
               d = self.bfs(forest, sr, sc, tr, tc, m, n, dr, dc)
               if d == -1:
                   return -1
               ans += d
               sr, sc = tr, tc
           return ans

       def bfs(self, forest, sr, sc, tr, tc, m, n, dr, dc):
           if sr == tr and sc == tc:
               return 0
           visited = [[False] * n for _ in range(m)]
           q = deque([(sr, sc, 0)])
           visited[sr][sc] = True

           while q:
               r, c, dist = q.popleft()
               for i in range(4):
                   nr, nc = r + dr[i], c + dc[i]
                   if 0 <= nr < m and 0 <= nc < n and not visited[nr][nc] and forest[nr][nc] != 0:
                       if nr == tr and nc == tc:
                           return dist + 1
                       visited[nr][nc] = True
                       q.append((nr, nc, dist + 1))
           return -1
