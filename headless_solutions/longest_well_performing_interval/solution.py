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
    def longestWPI(self, hours: List[int]) -> int:
        res, score = 0, 0
        seen = {}
        for i, h in enumerate(hours):
            score += 1 if h > 8 else -1
            if score > 0:
                res = i + 1
            else:
                if score not in seen:
                    seen[score] = i
                if score - 1 in seen:
                    res = max(res, i - seen[score - 1])
        return res
