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
    def solveNQueens(self, n: int) -> List[List[str]]:
        res, board = [], [['.'] * n for _ in range(n)]
        cols, d1, d2 = [False] * n, [False] * (2 * n), [False] * (2 * n)
        def backtrack(r):
            if r == n:
                res.append(["".join(row) for row in board])
                return
            for c in range(n):
                if not cols[c] and not d1[r - c + n] and not d2[r + c]:
                    board[r][c] = 'Q'
                    cols[c] = d1[r - c + n] = d2[r + c] = True
                    backtrack(r + 1)
                    board[r][c] = '.'
                    cols[c] = d1[r - c + n] = d2[r + c] = False
        backtrack(0)
        return res
