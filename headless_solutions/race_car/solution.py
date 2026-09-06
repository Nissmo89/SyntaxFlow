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
    def __init__(self):
        self.dp = {0: 0}

    def racecar(self, target: int) -> int:
        if target in self.dp:
            return self.dp[target]
        
        n = target.bit_length()
        if (1 << n) - 1 == target:
            self.dp[target] = n
        else:
            # Overshoot
            self.dp[target] = self.racecar((1 << n) - 1 - target) + n + 1
            # Undershoot
            for m in range(n - 1):
                self.dp[target] = min(self.dp[target], self.racecar(target - (1 << (n - 1)) + (1 << m)) + n + m + 1)
        
        return self.dp[target]
