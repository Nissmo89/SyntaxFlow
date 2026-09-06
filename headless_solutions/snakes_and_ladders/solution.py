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
    def snakesAndLadders(self, board: List[List[int]]) -> int:
        n = len(board)
        def get_pos(s):
            r, c = divmod(s - 1, n)
            row = n - 1 - r
            col = c if r % 2 == 0 else n - 1 - c
            return row, col
        
        q = deque([(1, 0)])
        visited = {1}
        while q:
            curr, dist = q.popleft()
            for i in range(1, 7):
                nxt = curr + i
                if nxt > n * n: break
                r, c = get_pos(nxt)
                if board[r][c] != -1: nxt = board[r][c]
                if nxt == n * n: return dist + 1
                if nxt not in visited:
                    visited.add(nxt)
                    q.append((nxt, dist + 1))
        return -1
