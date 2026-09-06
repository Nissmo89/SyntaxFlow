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
    def knightDialer(self, n: int) -> int:
        if n == 1: return 10
        MOD = 10**9 + 7
        dp = [1] * 10
        for _ in range(n - 1):
            new_dp = [0] * 10
            new_dp[0] = (dp[4] + dp[6]) % MOD
            new_dp[1] = (dp[6] + dp[8]) % MOD
            new_dp[2] = (dp[7] + dp[9]) % MOD
            new_dp[3] = (dp[4] + dp[8]) % MOD
            new_dp[4] = (dp[0] + dp[3] + dp[9]) % MOD
            new_dp[5] = 0
            new_dp[6] = (dp[0] + dp[1] + dp[7]) % MOD
            new_dp[7] = (dp[2] + dp[6]) % MOD
            new_dp[8] = (dp[1] + dp[3]) % MOD
            new_dp[9] = (dp[2] + dp[4]) % MOD
            dp = new_dp
        return sum(dp) % MOD
