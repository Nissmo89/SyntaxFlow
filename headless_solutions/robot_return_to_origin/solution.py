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
    def judgeCircle(self, moves: str) -> bool:
        x = 0
        y = 0
        for ch in moves:
            if ch == 'U':
                y += 1
            elif ch == 'D':
                y -= 1
            elif ch == 'R':
                x += 1
            elif ch == 'L':
                x -= 1
        return x == 0 and y == 0
