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
    def slidingPuzzle(self, board: List[List[int]]) -> int:
        target = "123450"
        start = "".join(str(c) for row in board for c in row)
        adj = {0: [1, 3], 1: [0, 2, 4], 2: [1, 5], 3: [0, 4], 4: [1, 3, 5], 5: [2, 4]}
        
        queue = deque([(start, 0)])
        visited = {start}
        
        while queue:
            curr, dist = queue.popleft()
            if curr == target: return dist
            zero = curr.index('0')
            for neighbor in adj[zero]:
                s_list = list(curr)
                s_list[zero], s_list[neighbor] = s_list[neighbor], s_list[zero]
                next_s = "".join(s_list)
                if next_s not in visited:
                    visited.add(next_s)
                    queue.append((next_s, dist + 1))
        return -1
