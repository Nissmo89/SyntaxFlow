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
    def spiralMatrixIII(self, rows: int, cols: int, rStart: int, cStart: int) -> List[List[int]]:
        res = []
        r, c = rStart, cStart
        dr, dc = [0, 1, 0, -1], [1, 0, -1, 0]
        step, di = 1, 0
        
        while len(res) < rows * cols:
            for _ in range(2):
                for _ in range(step):
                    if 0 <= r < rows and 0 <= c < cols:
                        res.append([r, c])
                    r, c = r + dr[di], c + dc[di]
                di = (di + 1) % 4
            step += 1
        return res
