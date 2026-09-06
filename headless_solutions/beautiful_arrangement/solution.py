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
    def countArrangement(self, n: int) -> int:
        bm = 0
        pl = n
        def dfs(bm, pl):
            if pl == 0:
                return 1
            S = 0
            for i in range(1, n + 1):
                if not bm & (1 << (i - 1)) and ((i % pl == 0) or (pl % i == 0)):
                    S += dfs(bm | (1 << (i - 1)), pl - 1)
            return S
        return dfs(bm, pl)
