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
    def lenLongestFibSubseq(self, arr: List[int]) -> int:
        val_to_idx = {x: i for i, x in enumerate(arr)}
        n = len(arr)
        dp = [[0] * n for _ in range(n)]
        max_len = 0
        
        for i in range(n):
            for j in range(i):
                diff = arr[i] - arr[j]
                if diff < arr[j] and diff in val_to_idx:
                    k = val_to_idx[diff]
                    dp[j][i] = dp[k][j] + 1
                    max_len = max(max_len, dp[j][i] + 2)
                    
        return max_len if max_len >= 3 else 0
