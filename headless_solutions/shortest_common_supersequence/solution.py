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
    def shortestCommonSupersequence(self, str1: str, str2: str) -> str:
        m, n = len(str1), len(str2)
        dp = [[0] * (n + 1) for _ in range(m + 1)]
        for i in range(m + 1): dp[i][0] = i
        for j in range(n + 1): dp[0][j] = j
        for i in range(1, m + 1):
            for j in range(1, n + 1):
                if str1[i-1] == str2[j-1]: dp[i][j] = 1 + dp[i-1][j-1]
                else: dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1])
        res, i, j = [], m, n
        while i > 0 and j > 0:
            if str1[i-1] == str2[j-1]: res.append(str1[i-1]); i -= 1; j -= 1
            elif dp[i-1][j] < dp[i][j-1]: res.append(str1[i-1]); i -= 1
            else: res.append(str2[j-1]); j -= 1
        res.extend(str1[:i][::-1])
        res.extend(str2[:j][::-1])
        return "".join(res[::-1])
