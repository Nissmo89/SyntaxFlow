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
    def numTilings(self, n: int) -> int:
        if n <= 2: return n
        if n == 3: return 5
        MOD = 10**9 + 7
        dp = [0] * (n + 1)
        dp[1], dp[2], dp[3] = 1, 2, 5
        for i in range(4, n + 1):
            dp[i] = (2 * dp[i - 1] + dp[i - 3]) % MOD
        return dp[n]
