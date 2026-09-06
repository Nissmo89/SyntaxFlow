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
    def openLock(self, deadends: list[str], target: str) -> int:
        dead = set(deadends)
        if "0000" in dead: return -1
        if target == "0000": return 0
        
        queue = deque([("0000", 0)])
        visited = {"0000"}
        
        while queue:
            curr, dist = queue.popleft()
            for i in range(4):
                digit = int(curr[i])
                for delta in [-1, 1]:
                    new_digit = (digit + delta) % 10
                    neighbor = curr[:i] + str(new_digit) + curr[i+1:]
                    if neighbor == target: return dist + 1
                    if neighbor not in dead and neighbor not in visited:
                        visited.add(neighbor)
                        queue.append((neighbor, dist + 1))
        return -1
