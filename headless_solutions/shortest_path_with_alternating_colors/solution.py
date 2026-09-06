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
    def shortestAlternatingPaths(self, n: int, redEdges: List[List[int]], blueEdges: List[List[int]]) -> List[int]:
        adj = [ [[] for _ in range(n)] for _ in range(2)]
        for u, v in redEdges: adj[0][u].append(v)
        for u, v in blueEdges: adj[1][u].append(v)
        
        dist = [[-1] * 2 for _ in range(n)]
        dist[0][0] = dist[0][1] = 0
        q = deque([(0, 0), (0, 1)])
        
        while q:
            u, c = q.popleft()
            next_c = 1 - c
            for v in adj[next_c][u]:
                if dist[v][next_c] == -1:
                    dist[v][next_c] = dist[u][c] + 1
                    q.append((v, next_c))
                    
        res = []
        for i in range(n):
            d = [x for x in dist[i] if x != -1]
            res.append(min(d) if d else -1)
        return res
