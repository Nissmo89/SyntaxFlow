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
    def checkRecord(self, n: int) -> int:
        MOD = 10**9 + 7
        # dp[absences][trailing_lates]
        dp = [[0] * 3 for _ in range(2)]
        dp[0][0] = 1
        
        for _ in range(n):
            new_dp = [[0] * 3 for _ in range(2)]
            for a in range(2):
                for l in range(3):
                    # Add 'P'
                    new_dp[a][0] = (new_dp[a][0] + dp[a][l]) % MOD
                    # Add 'L'
                    if l + 1 < 3:
                        new_dp[a][l + 1] = (new_dp[a][l + 1] + dp[a][l]) % MOD
                    # Add 'A'
                    if a + 1 < 2:
                        new_dp[a + 1][0] = (new_dp[a + 1][0] + dp[a][l]) % MOD
            dp = new_dp
            
        return sum(sum(row) for row in dp) % MOD
