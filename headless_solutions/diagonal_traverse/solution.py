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
    def findDiagonalOrder(self, mat: List[List[int]]) -> List[int]:
        if not mat or not mat[0]: return []
        m, n = len(mat), len(mat[0])
        res = [0] * (m * n)
        r = c = 0
        for i in range(m * n):
            res[i] = mat[r][c]
            if (r + c) % 2 == 0: # Moving Up
                if c == n - 1: r += 1
                elif r == 0: c += 1
                else: r -= 1; c += 1
            else: # Moving Down
                if r == m - 1: c += 1
                elif c == 0: r += 1
                else: r += 1; c -= 1
        return res
