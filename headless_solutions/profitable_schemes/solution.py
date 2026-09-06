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
    def profitableSchemes(self, n: int, minProfit: int, group: List[int], profit: List[int]) -> int:
        MOD = 10**9 + 7
        dp = [[0] * (n + 1) for _ in range(minProfit + 1)]
        dp[0][0] = 1
        
        for p_i, g_i in zip(profit, group):
            for i in range(minProfit, -1, -1):
                for j in range(n - g_i, -1, -1):
                    next_p = min(i + p_i, minProfit)
                    dp[next_p][j + g_i] = (dp[next_p][j + g_i] + dp[i][j]) % MOD
                    
        return sum(dp[minProfit]) % MOD
