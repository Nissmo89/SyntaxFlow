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
    def minCut(self, s: str) -> int:
        n = len(s)
        cuts = [i for i in range(n)]
        for i in range(n):
            # Odd length
            l, r = i, i
            while l >= 0 and r < n and s[l] == s[r]:
                cuts[r] = min(cuts[r], 0 if l == 0 else cuts[l - 1] + 1)
                l -= 1; r += 1
            # Even length
            l, r = i, i + 1
            while l >= 0 and r < n and s[l] == s[r]:
                cuts[r] = min(cuts[r], 0 if l == 0 else cuts[l - 1] + 1)
                l -= 1; r += 1
        return cuts[n - 1]
