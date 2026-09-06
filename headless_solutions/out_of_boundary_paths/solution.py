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
    def findPaths(self, m: int, n: int, maxMove: int, startRow: int, startColumn: int) -> int:
        MOD = 10**9 + 7
        dp = [[0] * n for _ in range(m)]
        dp[startRow][startColumn] = 1
        ans = 0
        
        for _ in range(maxMove):
            new_dp = [[0] * n for _ in range(m)]
            for r in range(m):
                for c in range(n):
                    if dp[r][c] > 0:
                        for dr, dc in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                            nr, nc = r + dr, c + dc
                            if not (0 <= nr < m and 0 <= nc < n):
                                ans = (ans + dp[r][c]) % MOD
                            else:
                                new_dp[nr][nc] = (new_dp[nr][nc] + dp[r][c]) % MOD
            dp = new_dp
        return ans
