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

import bisect

class Solution:
    def findRightInterval(self, intervals: List[List[int]]) -> List[int]:
        n = len(intervals)
        # Store (start, original_index) and sort by start
        starts = sorted([(intervals[i][0], i) for i in range(n)])
        start_vals = [s[0] for s in starts]
        
        res = []
        for _, end in intervals:
            idx = bisect.bisect_left(start_vals, end)
            if idx < n:
                res.append(starts[idx][1])
            else:
                res.append(-1)
        return res
