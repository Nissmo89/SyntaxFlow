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
    def minWindow(self, s: str, t: str) -> str:
        from collections import Counter
        need = Counter(t)
        missing = len(t)
        i = 0
        res = (0, float('inf'))
        for j, char in enumerate(s):
            if need[char] > 0:
                missing -= 1
            need[char] -= 1
            while missing == 0:
                if j - i < res[1] - res[0]:
                    res = (i, j)
                need[s[i]] += 1
                if need[s[i]] > 0:
                    missing += 1
                i += 1
        return "" if res[1] == float('inf') else s[res[0]:res[1]+1]
