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
    def totalNQueens(self, n: int) -> int:
        cols, d1, d2 = [False] * n, [False] * (2 * n), [False] * (2 * n)
        self.count = 0
        def backtrack(row):
            if row == n:
                self.count += 1
                return
            for col in range(n):
                if not (cols[col] or d1[row - col + n] or d2[row + col]):
                    cols[col] = d1[row - col + n] = d2[row + col] = True
                    backtrack(row + 1)
                    cols[col] = d1[row - col + n] = d2[row + col] = False
        backtrack(0)
        return self.count
