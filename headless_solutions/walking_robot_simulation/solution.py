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
    def robotSim(self, commands: List[int], obstacles: List[List[int]]) -> int:
        obs = set(map(tuple, obstacles))
        dx, dy = [0, 1, 0, -1], [1, 0, -1, 0]
        x, y, d, res = 0, 0, 0, 0
        for c in commands:
            if c == -1: d = (d + 1) % 4
            elif c == -2: d = (d + 3) % 4
            else:
                for _ in range(c):
                    if (x + dx[d], y + dy[d]) in obs: break
                    x, y = x + dx[d], y + dy[d]
                    res = max(res, x*x + y*y)
        return res
