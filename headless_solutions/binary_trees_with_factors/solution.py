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
    def numFactoredBinaryTrees(self, arr: List[int]) -> int:
        mod = 10**9 + 7
        dp = {}
        arr.sort()
        for a in arr:
            dp[a] = 1
            for b in arr[:a]:
                if a % b == 0:
                    c = a // b
                    if c in dp:
                        dp[a] = (dp[a] + dp[b] * dp[c]) % mod
        return sum(dp.values()) % mod
