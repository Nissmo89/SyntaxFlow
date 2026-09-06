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
    def reachableNodes(self, edges: list[list[int]], maxMoves: int, n: int) -> int:
        adj = [[] for _ in range(n)]
        for u, v, cnt in edges:
            adj[u].append((v, cnt))
            adj[v].append((u, cnt))
        
        pq = [(-maxMoves, 0)]
        dist = [-1] * n
        dist[0] = maxMoves
        count = 0
        
        while pq:
            moves, u = heapq.heappop(pq)
            moves = -moves
            if moves < dist[u]: continue
            count += 1
            for v, cnt in adj[u]:
                remaining = moves - cnt - 1
                if remaining >= 0 and remaining > dist[v]:
                    dist[v] = remaining
                    heapq.heappush(pq, (-remaining, v))
        
        for u, v, cnt in edges:
            a = max(0, dist[u])
            b = max(0, dist[v])
            count += min(cnt, a + b)
        return count
