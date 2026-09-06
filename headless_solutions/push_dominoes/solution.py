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
    def pushDominoes(self, dominoes: str) -> str:
        s = 'L' + dominoes + 'R'
        res = []
        i = 0
        for j in range(1, len(s)):
            if s[j] == '.': continue
            mid = j - i - 1
            if i > 0: res.append(s[i])
            if s[i] == s[j]: res.append(s[i] * mid)
            elif s[i] == 'L' and s[j] == 'R': res.append('.' * mid)
            else: res.append('R' * (mid // 2) + '.' * (mid % 2) + 'L' * (mid // 2))
            i = j
        return "".join(res)
