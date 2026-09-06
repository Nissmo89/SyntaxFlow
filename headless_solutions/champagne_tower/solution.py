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
    def champagneTower(self, poured: int, query_row: int, query_glass: int) -> float:
        # dp[r][c] holds the total champagne that has flowed through glass (r,c)
        dp = [[0.0] * (query_row + 2) for _ in range(query_row + 2)]
        dp[0][0] = float(poured)

        for r in range(query_row + 1):
            for c in range(r + 1):
                if dp[r][c] > 1.0:
                    overflow = (dp[r][c] - 1.0) / 2.0
                    dp[r + 1][c]     += overflow
                    dp[r + 1][c + 1] += overflow
                    dp[r][c] = 1.0

        return dp[query_row][query_glass]
