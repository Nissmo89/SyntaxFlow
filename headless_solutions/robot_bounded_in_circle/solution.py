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
    def isRobotBounded(self, instructions: str) -> bool:
        x, y = 0, 0
        # Directions: N, E, S, W
        dx, dy = [0, 1, 0, -1], [1, 0, -1, 0]
        dir_idx = 0
        
        for char in instructions:
            if char == 'G':
                x, y = x + dx[dir_idx], y + dy[dir_idx]
            elif char == 'L':
                dir_idx = (dir_idx - 1) % 4
            else:
                dir_idx = (dir_idx + 1) % 4
                
        return (x == 0 and y == 0) or (dir_idx != 0)
