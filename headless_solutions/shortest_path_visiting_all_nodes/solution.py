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
    def shortestPathLength(self, graph: list[list[int]]) -> int:
        n = len(graph)
        if n == 1: return 0
        target = (1 << n) - 1
        queue = deque([(1 << i, i, 0) for i in range(n)])
        visited = {(1 << i, i) for i in range(n)}
        
        while queue:
            mask, node, dist = queue.popleft()
            for neighbor in graph[node]:
                new_mask = mask | (1 << neighbor)
                if new_mask == target: return dist + 1
                if (new_mask, neighbor) not in visited:
                    visited.add((new_mask, neighbor))
                    queue.append((new_mask, neighbor, dist + 1))
        return 0
