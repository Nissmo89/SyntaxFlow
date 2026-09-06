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
    def findCheapestPrice(self, n: int, flights: List[List[int]], src: int, dst: int, k: int) -> int:
        INF = float('inf')
        dist = [INF] * n
        dist[src] = 0
        
        for _ in range(k + 1):
            temp = dist[:]  # copy current best
            updated = False
            for u, v, w in flights:
                if dist[u] != INF:
                    if dist[u] + w < temp[v]:
                        temp[v] = dist[u] + w
                        updated = True
            dist = temp
            if not updated:
                break
        
        return dist[dst] if dist[dst] != INF else -1
