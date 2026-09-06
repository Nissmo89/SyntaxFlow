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
    def numSubmatrixSumTarget(self, matrix: List[List[int]], target: int) -> int:
        m, n = len(matrix), len(matrix[0])
        for r in range(m):
            for c in range(1, n):
                matrix[r][c] += matrix[r][c - 1]
        
        res = 0
        for i in range(n):
            for j in range(i, n):
                d = {0: 1}
                cur = 0
                for r in range(m):
                    cur += matrix[r][j] - (matrix[r][i - 1] if i > 0 else 0)
                    res += d.get(cur - target, 0)
                    d[cur] = d.get(cur, 0) + 1
        return res
