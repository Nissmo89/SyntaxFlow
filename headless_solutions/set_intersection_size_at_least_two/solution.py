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
    def intersectionSizeTwo(self, intervals: List[List[int]]) -> int:
        intervals.sort(key=lambda x: (x[1], -x[0]))
        p1, p2, ans = -1, -1, 0
        for s, e in intervals:
            if s > p2:
                ans += 2
                p1, p2 = e - 1, e
            elif s > p1:
                ans += 1
                p1, p2 = p2, e
        return ans
