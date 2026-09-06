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
    def soupServings(self, n: int) -> float:
        if n > 5000: return 1.0
        memo = {}
        def dp(a, b):
            if a <= 0 and b <= 0: return 0.5
            if a <= 0: return 1.0
            if b <= 0: return 0.0
            if (a, b) in memo: return memo[(a, b)]
            memo[(a, b)] = 0.25 * (dp(a-4, b) + dp(a-3, b-1) + dp(a-2, b-2) + dp(a-1, b-3))
            return memo[(a, b)]
        return dp((n + 24) // 25, (n + 24) // 25)
